#!/usr/bin/perl


use strict;
use warnings;
use CGI;

my $cgi = new CGI;

print $cgi->header();
print $cgi->start_html("save_data");

print $cgi->end_html();

#print "Content-type: text/html\n\n"; 
#print "hello world!!!";
exit
