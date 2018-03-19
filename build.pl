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
use LWP::UserAgent;
use File::Fetch;

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
sub DownloadFile
{
    my ($file_name)=@_;

	print ("> call DownloadFile with args: $file_name\n");
	
	my $url="";
	if ($osname eq 'MSWin32')
	{
		$url="https://docs.google.com/uc?export=download&id=1UFhm5RJe3IjnK9DPxzNvE8VccCg-Zo7c";
	}
	if ($osname eq 'linux')
	{
		$url="https://docs.google.com/uc?export=download&id=1vjiU9jbtFgJBwWypQ_JPrF2lY3GUqb_s";
	}
	if ($osname eq 'darwin')
	{
		$url="https://docs.google.com/uc?export=download&id=1e7Lg9FkaC2YYDtekwhSw1t1jaVxmnnjT";
	}

	my $ua = LWP::UserAgent->new;
	my $response = $ua->get($url);
	die $response->status_line if !$response->is_success;
	print "HTTP response code was: ", $response->status_line, "\n";
	my $file = $response->decoded_content( charset => 'none' );
	getstore($url,$file_name);	
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
	DownloadFile("$scriptDir/libs.zip");
	UnzipFile("$scriptDir/libs.zip", "$scriptDir/src/");
	unlink "$scriptDir/libs.zip";

    # create build directory
    my $directory = "build";
    unless(-e $directory or mkdir $directory)
    {
        die "Unable to create $directory\n";
    }
	print ("scriptDir > $scriptDir\n");
	
    my $buildDir="$scriptDir/$directory";
	
    if ($osname eq 'MSWin32')
    {
        #call cmake
        my $CmakeCmd='cmake .. -G "Visual Studio 14 2015 Win64"';
		my $systemCmd="cd $buildDir && $CmakeCmd";
        print ("> $systemCmd\n");
        VerboseMessage("> $systemCmd");
        system($systemCmd);

        #call Visual Studio to build project
        my $VSBuildCmd='"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" /m /p:configuration=Release build\ALL_BUILD.vcxproj';
        print ("> $VSBuildCmd\n");
        VerboseMessage("> $VSBuildCmd");
        system($VSBuildCmd);
    }
    else
    {
        #call cmake
        my $CmakeCmd="cmake ..";
        print ("> $CmakeCmd\n");
        VerboseMessage("> $CmakeCmd");
        system($CmakeCmd);

        #call make
        my $MakeCmd="make";
        print ("> $MakeCmd\n");
        VerboseMessage("> $MakeCmd");
        system($MakeCmd);
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


