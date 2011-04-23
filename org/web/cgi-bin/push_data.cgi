#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $cgi = new CGI;

print "Content-type:text/plain\n\n";
print $cgi->param("mid"), "\n";
print $cgi->param("pid"), "\n";
print $cgi->param("data"), "\n", time();

# get the paramterss
my $mid = $cgi->param("mid");
my $pid = $cgi->param("pid");
my $data = $cgi->param("data");
my $time = time();

open(DATA_FILE, sprintf(">>data/%04d-%04d", $pid, $mid));
print DATA_FILE $time," ",$data,"\n";
close(DATA_FILE);



