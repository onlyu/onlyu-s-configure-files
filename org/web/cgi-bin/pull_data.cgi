#!/usr/bin/perl 

use strict;
use warnings;
use CGI;

my $cgi = new CGI;

my $mid = $cgi->param("mid");
my $pid = $cgi->param("pid");

if(not defined($mid)){
    $mid = 1;
    $pid = 2;
}

# send the content to client
print "Content-type:text/plain\n\n";
my $file_name = sprintf("data/%04d-%04d", $pid, $mid);
open(DATA_FILE,"<".$file_name) or die "not such file";
while(<DATA_FILE>){
    chomp($_);
    my($time,$data) = split(/ /, $_);
    print $pid,",",$mid,",",$time,",",$data,"\n";
}
close(DATA_FILE);

# delete the file
unlink $file_name;
