#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $cgi = new CGI;

print "Content-type:text/plain\n\n";
print $cgi->param("id"), "\n", $cgi->param("data"), "\n", time();

my $id = $cgi->param("id");
my $data = $cgi->param("data");
my $time = time();


