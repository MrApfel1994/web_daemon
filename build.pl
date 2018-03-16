#!/usr/bin/perl

use Cwd;
use File::Basename;
use Cwd 'abs_path';
use Data::Dumper;
use File::Spec 'abs2rel';
use Class::Struct;
use FindBin qw($Bin);
use File::chdir;

#-----------------------------------------------------------------------
# check for OS
#-----------------------------------------------------------------------
my $osname=$^O;
my $verbose = 0;
my $currentWorkingDir = getcwd();

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
    my ($id, $file_name) = @_;
    my $url="https://docs.google.com/uc?export=download&id="+id;
}

#-----------------------------------------------------------------------
# unpack zip file
#-----------------------------------------------------------------------
sub UnzipFile
{
    my ($file_name, $unzip_dest) = @_;
}

#-----------------------------------------------------------------------
# build content
#-----------------------------------------------------------------------
sub Build
{
    my $pythonDownloadCmd="python download.py";
    print ("> $pythonDownloadCmd\n");
    VerboseMessage("> $pythonDownloadCmd");
    system($pythonDownloadCmd);

    # create build directory
    my $directory = "build";
    unless(-e $directory or mkdir $directory)
    {
        die "Unable to create $directory\n";
    }
    my $buildDir=$currentWorkingDir/$directory;
    # change to build directory
    local $CWD = '$buildDir';

    if ($osname eq 'MSWin32')
    {
        #call cmake
        my $CmakeCmd='cmake .. -G "Visual Studio 15 2017 Win64"';
        print ("> $CmakeCmd\n");
        VerboseMessage("> $CmakeCmd");
        system($CmakeCmd);

        #call Visual Studio to build project
        my $VSBuildCmd='"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" /m /p:configuration=Release /p:platform=x64 /t:clean $buildDir\build\ALL_BUILD.vcxproj';
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


