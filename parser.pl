#! /usr/bin/perl
use strict;

my @file = ("resources/bunny.obj",  "resources/dragon.obj", "resources/bear.obj", "resources/teapot.obj", "resources/teddy.obj", "resources/cow.obj");

foreach my $file(@file){
	generate_header($file);
}
sub generate_header{
				my ($file) = @_;
				my ($out_file, $out_file_header);
				my ($name, $fh, $fh_out, $fh_out_header);
				my (@vertice, @normal, @numbers);
				my (@vertice_index, @normal_index);
				$file =~ /\/(\w+)\.obj/;

				$name = $1;
				$out_file_header = "./components/include/$name"."_model.h";
				$out_file = "./components/source/$name"."_model.cpp";
				open($fh, "<", $file); 
				open($fh_out, ">", $out_file) or die "$!";
				open($fh_out_header, " >", $out_file_header) or die "$!";

				print "Processing $name...\n";
								while(<$fh>){
					my ($size, $line, $type);
					
					$line = $_;
					
					($type, @numbers) = split('\s+', $line);
					
					if($type eq "#"){
						next;					} elsif($type eq "v") {
						$size = @numbers;
						
						if($size != 6){
							push @vertice, [@numbers, 0.95, 0.95, 0.95];						} else {
							push @vertice, [ @numbers];
						}					} elsif($type eq "vn") {
						my ($x, $y, $z) = @numbers;
						my $length = sqrt($x * $x + $y * $y + $z * $z);
						if($length > 0.1){
								push @normal, [$x / $length, $y / $length, $z / $length];
								}
								else {
									push @normal, [0.001, 0.001, 0.001];
								}					} elsif($type eq "f") {
						$size = @numbers;
						
						foreach my $vertice(@numbers){
							my ($v, $t, $n) = split('/', $vertice);
							$v = $v-1;
							$n = $n-1;
							push @vertice_index, $v;
							push @normal_index, $n;						}							}
				}

				print $fh_out "#include \"DataType.h\"\n#include \"$name"."_model.h\"\n\n";
				# Vertice
				my $vertice_array = "$name"."_vertice";
				my $vertice_array_size = "$name"."_n_vertice";

				print $fh_out "float $vertice_array [] = {";
				foreach my $v(@vertice){
					print $fh_out "@$v[0], @$v[1], @$v[2], @$v[3], @$v[4], @$v[5],\n";
				}
				print $fh_out "};\n\n";
				print $fh_out "UINT32 $vertice_array_size = sizeof($vertice_array) / sizeof(float);";

				# Normal
				my $normal_array = "$name"."_normal";
				my $normal_array_size = "$name"."_n_normal";

				print $fh_out "float $normal_array [] = {";
				foreach my $v(@normal){
					print $fh_out "@$v[0], @$v[1], @$v[2],\n";
				}
				print $fh_out "};\n\n";
				print $fh_out "UINT32 $normal_array_size = sizeof($normal_array) / sizeof(float);\n\n";
				# Index
				my $face_array = "$name" ."_face" ;
				my $face_array_size = "$name" ."_n_face";
				
				print $fh_out "UINT32 $face_array [] = {";
				foreach my $v(@vertice_index){
								print $fh_out "$v,\n";
				}
				print $fh_out "};\n\n";
				print $fh_out "UINT32 $face_array_size = sizeof($face_array) / sizeof(UINT32);";

				# Header File
				print $fh_out_header  "#pragma once

#include \"DataType.h\"

extern float $vertice_array [];
extern float $normal_array [];
extern UINT32 $face_array [];
extern UINT32 $vertice_array_size;
extern UINT32 $normal_array_size;
extern UINT32 $face_array_size;";
				next;				# Face
				my $vertice_array = "$name"."_vertice";
				my $vertice_array_size = "$name"."_n_vertice";

				print $fh_out "#include \"DataType.h\"\n\n";
				print $fh_out "extern float $vertice_array [] = {";
				print @vertice_index."\n";
				for (my $i = 0; $i < @vertice_index; $i++){
					my $v_i = $vertice_index[$i];
					my $n_i = $normal_index[$i];
					my $v = $vertice[$v_i];
					my $n = $normal[$n_i];
					
					print $fh_out "@$v[0]f, @$v[1]f, @$v[2]f, @$v[3]f, @$v[4]f, @$v[5]f, @$n[0]f, @$n[1]f, @$n[2]f,\n";				}

				print $fh_out "};\n\n";
				print $fh_out "extern UINT64 $vertice_array_size = sizeof($vertice_array) / sizeof(float) / 9;";
}exit;
# Vertice
# my $vertice_array = "$name"."_vertice";
# my $vertice_array_size = "$name"."n_vertice";
# print $fh_out "extern float $vertice_array [] = {";
# foreach my $v(@vertice){
	# print $fh_out "@$v[0]f, @$v[1]f, @$v[2]f, @$v[3]f, @$v[4]f, @$v[5]f,\n";# }
# print $fh_out "};\n";
# print $fh_out "extern UINT64 $vertice_array_size = sizeof($vertice_array) / sizeof(float);";


# # Normal
# my $normal_array = "$name"."_normal";
# my $normal_array_size = "$name"."_n_normal";
# print $fh_out "extern float $normal_array [] = {";
# foreach my $v(@normal){
	# print $fh_out "@$v[0]f, @$v[1]f, @$v[2]f,\n";
# }# print $fh_out "};\n";
# print $fh_out "extern UINT64 $normal_array_size = sizeof($normal_array) / sizeof(float);";

# my $index_array = "$name"."_normal";
# my $normal_array_size = "$name"."_n_normal";

# print $fh_out "extern float $normal_array [] = {";
# foreach my $v(@normal){
	# print $fh_out "@$v[0]f, @$v[1]f, @$v[2]f,\n";
# }
# print $fh_out "};\n";
# print $fh_out "extern UINT64 $normal_array_size = sizeof($normal_array) / sizeof(float);";