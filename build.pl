#!/usr/bin/perl

use Cwd;
use File::Basename;
use Cwd 'abs_path';
use Data::Dumper;
use File::Spec 'abs2rel';
use Class::Struct;
use FindBin qw($Bin);
use Archive::Zip qw( :ERROR_CODES :CONSTANTS );
use LWP::Simple qw(getstore);
use LWP::ConnCache;
use LWP::UserAgent;
use File::Fetch;
use File::Copy;

#-----------------------------------------------------------------------
# check for OS
#-----------------------------------------------------------------------
my $osname=$^O;
my $verbose = 0;
my $scriptDir = dirname(abs_path($0));


for $arg (@ARGV)
{
    if (($arg eq "-v")||($arg eq "-verbose"))
    {
        $verbose = 1;
    }
}

{
    Build();
}

#-----------------------------------------------------------------------
# download zip file
#-----------------------------------------------------------------------
sub DownloadFileFromGoogleDocs
{
    my ($file_name)=@_;

    print ("> call DownloadFile with args: $file_name\n");

    my $url="";
    if ($osname eq 'MSWin32' or $osname eq 'msys')
    {
        $url="https://docs.google.com/uc?export=download&id=1lM9fgdOkMmYX5j7AvrsqFnHYyKvt6k9J";
    }
    if ($osname eq 'linux')
    {
        $url="https://docs.google.com/uc?export=download&id=1PceSFgLAHITcHRmyIJdEBAk5916fnPp_";
    }
    if ($osname eq 'darwin')
    {
        $url="https://docs.google.com/uc?export=download&id=1e7Lg9FkaC2YYDtekwhSw1t1jaVxmnnjT";
    }

    print "download url is: $url\n";

    my $confirm_token = "None";
    my $cache = LWP::ConnCache->new;
    $cache->total_capacity(2); # Cache up to 2 connections
    my $ua = LWP::UserAgent->new(conn_cache => $cache);
    my $response = $ua->get($url);

    print "HTTP response code was: ", $response->status_line, "\n";
    if ($response->is_success)
    {
        # check header for content-type 
        $type = $response->content_type( );
        print "content type: $type\n";
        if ($type eq "text/html" )
        {
            print "confirmation is needed -> load confirmation link from respons and proceed with download \n";		
            my $buffer = $response->content();
            my $indexOfConfirmStr = index($buffer, "confirm=");
            if ($indexOfConfirmStr >= 0)
            {
                print "indexOfConfirmStr: $indexOfConfirmStr \n";
                my $indexOfEndConfirmStr = index($buffer, "&", $indexOfConfirmStr);
                if ($indexOfEndConfirmStr >= 0)
                {
                    print "indexOfEndConfirmStr: $indexOfEndConfirmStr \n";
                    # extract token
                    $confirm_token = substr($buffer, $indexOfConfirmStr, $indexOfEndConfirmStr-$indexOfConfirmStr);
                    print "confirm_token: $confirm_token \n";
                }
            }
        }

        if($confirm_token eq "None")
        {
            getstore($url,$file_name);
        }
        if($confirm_token ne "None")
        {
            my $indexOfId = index($url, "&id=");
            print "indexOfId: $indexOfId \n";
            my $newUrl = join "",substr($url, 0, $indexOfId),"&",$confirm_token,substr($url, $indexOfId);
            print "newUrl: $newUrl \n";

            my $newresponse = $ua->get($newUrl);
            print "HTTP response code was: ", $newresponse->status_line, "\n";
            if ($newresponse->is_success)
            {
                $type = $newresponse->content_type( );
                print "content type: $type\n";
                if ($type ne "text/html" )
                {
                    getstore($newUrl,$file_name);
                }
                else
                {
                    print "not possible to download file because manual confirmation needed.\n";
                }
            }
        }
    }

    sleep(1);
}

#-----------------------------------------------------------------------
# unpack zip file
#-----------------------------------------------------------------------
sub UnzipFile
{
    my ($file_name, $unzip_dest) = @_;
    print ("> call UnzipFile with args: $file_name $unzip_dest\n");
    my $zip = Archive::Zip->new();
    unless ( $zip->read($file_name) == AZ_OK )
    {
        die 'read error';
    }

    $zip->extractTree("","$unzip_dest");
}

#-----------------------------------------------------------------------
# build content
#-----------------------------------------------------------------------
sub Build
{
    #my $pythonDownloadCmd="python download.py";
    #print ("> $pythonDownloadCmd\n");
    #VerboseMessage("> $pythonDownloadCmd");
    #system($pythonDownloadCmd);

    my $dstFile = ToOsPath("$scriptDir/libs.zip");
    if ($osname eq 'MSWin32' or $osname eq 'msys')
    {
        #for windows we copy the file from our \\LUMEN\MA server
        my $srcFile = "\\\\malighting\\ma\\Software\\Projekte\\webdaemon_libs\\win_libs.zip";
        print ("> copy($srcFile, $dstFile)\n");
        unless(copy($srcFile, $dstFile))
        {
            #network path was not available -> try to access path via network-drive (normally the case for build server)
            my $netSrcFile = ToOsPath("S:/Projekte/webdaemon_libs/win_libs.zip");
            copy($netSrcFile, $dstFile) or die "Copy Failed: $!\n";
        }
    }
    elsif ($osname eq 'linux')
    {
        #for linux we also copy the file from our MA server (wbb.malighting.de) -> ma dir from server should be mounted to /media/ma_server/
        $srcFile = "/media/ma_server/Software/Projekte/webdaemon_libs/lin_libs.zip";
        print ("> copy($srcFile, $dstFile)\n");
        die "Copy Failed: $!\n" unless(copy($srcFile, $dstFile));
    }
    else
    {
        DownloadFileFromGoogleDocs($dstFile);
    }
    UnzipFile($dstFile, ToOsPath("$scriptDir/src/"));
    unlink $dstFile;

    # create build directory
    my $directory = "build";
    unless(-e $directory or mkdir $directory)
    {
        die "Unable to create $directory\n";
    }

    my $buildDir=ToOsPath("$scriptDir/$directory/");
    print ("buildDir > $buildDir\n");

    if ($osname eq 'MSWin32' or $osname eq 'msys')
    {
        #call cmake
        my $CmakeCmd='cmake .. -G "Visual Studio 14 2015 Win64"';
        my $systemCmd="cd $buildDir && $CmakeCmd";
        print ("> $systemCmd\n");
        VerboseMessage("> $systemCmd");
        system($systemCmd);

        #call Visual Studio to build project
        #my $msbuildPath=ToOsPath("C:\\Program Files (x86)\\MSBuild\\14.0\\Bin\\MSBuild.exe");
        #my $VSBuildCmd="'$msbuildPath' /m /p:configuration=Release build\\ALL_BUILD.vcxproj";
        my $VSBuildCmd='"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" /m /p:configuration=Release build\ALL_BUILD.vcxproj';
        print ("> $VSBuildCmd\n");
        VerboseMessage("> $VSBuildCmd");
        system($VSBuildCmd);

        my $upxPath=ToOsPath("src/libs/upx.exe");
        my $finishCmd="$upxPath --best web_daemon.exe";
        print ("> $finishCmd\n");
        VerboseMessage("> $finishCmd");
        #system($finishCmd);
    }
    else
    {
        #call cmake
        my $CmakeCmd="cmake ..";
        my $systemCmd="cd $buildDir && $CmakeCmd";
        print ("> $systemCmd\n");
        VerboseMessage("> $systemCmd");
        system($systemCmd);

        #call make
        my $MakeCmd="make";
        $systemCmd="cd $buildDir && $MakeCmd";
        print ("> $systemCmd\n");
        VerboseMessage("> $systemCmd");
        system($systemCmd);

        my $upxPath=ToOsPath("./src/libs/upx");
        my $webDaemonBinaryPath=ToOsPath("./web_daemon");

        system("strip -s $webDaemonBinaryPath");

        # first make sure we can execute upx -> change rights
        system("chmod +x $upxPath");
        my $finishCmd="$upxPath $webDaemonBinaryPath";
        print ("> $finishCmd\n");
        VerboseMessage("> $finishCmd");
        system($finishCmd);
    }
}

sub VerboseMessage()
{
    my ($message)=@_;
    if ($verbose!=0)
    {
        print "$message\n";
    }
}

sub ToOsPath()
{
    my ($path) = @_;
    if ($osname ne 'linux')
    {
        $path =~ s/^\/([a-zA-Z])/$1:/;
        $path =~ s/\//\\/g;
    }
    return $path;
}
