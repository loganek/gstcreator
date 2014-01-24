#!/usr/bin/perl

# states:
# 0 - no enum found
# 1 - enum class declaration found
# 2 - `{` found
# 3 - `,` not found in enum value (last value in enum)

sub syntax_error($$) {
	my $line_number = shift;
	my $message     = shift;
	print "Syntax error on line " . $line_number . ": " . $message . "\n";
	exit;
}

sub state_error($$$) {
	my $line_number = shift;
	my $expected    = shift;
	my $current     = shift;

	my $message = "expected state `$expected`, but `$current` state found";

	syntax_error( $line_number, $message );
}

sub generate_converter {
	my ( $enum_name, @enum_values ) = @_;

	$ret = "template<>\n";
	$ret .=
"std::string EnumUtils<$enum_name>::enum_to_string($enum_name enum_value)\n";
	$ret .= "{\n";
	$ret .= "\tswitch (enum_value)\n";
	$ret .= "\t{\n";

	for ( my $i = 0 ; $i < @enum_values ; $i++ ) {
		$ret .= "\tcase $enum_name\::$enum_values[$i]:\n";
		$ret .= "\t\treturn \"$enum_values[$i]\";\n";
	}

	$ret .= "\tdefault:\n";
	$ret .= "\t\t throw std::runtime_error(\"unknow enum value\");\n";
	$ret .= "\t}\n";
	$ret .= "}\n\n";

	$ret .= "template<>\n";
	$ret .=
	  "std::vector<std::string> EnumUtils<$enum_name>::get_string_values()\n";
	$ret .= "{\n";
	$ret .= "\tstd::vector<std::string> values = {";

	for ( my $i = 0 ; $i < @enum_values ; $i++ ) {
		$ret .= "\"$enum_values[$i]\",";
	}
	$ret .= "};\n\n";
	$ret .= "\t return values;\n";
	$ret .= "}\n\n";

	$ret .= "template<>\n";
	$ret .=
"$enum_name EnumUtils<$enum_name>::string_to_enum(const std::string& enum_value)\n";
	$ret .= "{\n";
	$ret .= "\tstd::string val = StringUtils::to_upper(enum_value);\n\n";

	for ( my $i = 0 ; $i < @enum_values ; $i++ ) {
		$ret .= "\tif (val == \"$enum_values[$i]\")\n";
		$ret .= "\t\treturn $enum_name\::$enum_values[$i];\n";
	}

	$ret .= "\n\tthrow std::runtime_error(\"cannot find enum value\");\n";
	$ret .= "}\n\n";

	return $ret;
}

sub trim($) {
	my $s = shift;
	$s =~ s/^\s+//;
	$s =~ s/\s+$//;
	return $s;
}

$arg_cnt = $#ARGV + 1;

# TODO file list should be allowed
if ( $arg_cnt != 3 ) {
	print
"\nUsage: enmu_string_generator.pl <enum's filename> <output header filename> <output source filename>\n";
	exit;
}

my $autogenerate_string = "// auto-generated file! Don't modify it!\n\n";
my $state = 0;    # TODO check enum
my $enum_name;
my @enum_values = ();

open my $input_file,      "<", $ARGV[0] or die $!;
open my $output_file,     ">", $ARGV[1] or die $!;
open my $output_file_cpp, ">", $ARGV[2] or die $!;

$ifdef_str = uc( $ARGV[1] );
$ifdef_str =~ s/[^a-zA-Z\d]+/_/g;

print $output_file $autogenerate_string;
print $output_file "#ifndef " . $ifdef_str . "\n";
print $output_file "#define " . $ifdef_str . "\n\n";

print $output_file "#include <vector>\n";
print $output_file "#include <string>\n\n";

print $output_file "template<typename ENUM_TYPE>\n";
print $output_file "class EnumUtils\n";
print $output_file "{\n";
print $output_file "public:\n";
print $output_file "\tstatic ENUM_TYPE string_to_enum(const std::string&);\n";
print $output_file "\tstatic std::string enum_to_string(ENUM_TYPE);\n";
print $output_file "\tstatic std::vector<std::string> get_string_values();\n";
print $output_file "};\n";
print $output_file "\n#endif\n";

print $output_file_cpp "#include \"$ARGV[1]\"\n";
print $output_file_cpp "#include \"Utils/StringUtils.h\"\n";
print $output_file_cpp "#include <stdexcept>\n\n";

if ( $ARGV[0] =~ m/include\/(.*)$/ ) {
	$include_str = $1;
}
else {
	$include_str = $ARGV[0];
}

print $output_file_cpp "#include \"$include_str\"\n\n";

while ( my $line = <$input_file> ) {
	$line = trim($line);
	if ( $line =~ m/enum\s*class\s*(.+)$/ ) {
		if ( $state != 0 ) {
			state_error( $., 0, $state );
		}
		$state     = 1;
		$enum_name = $1;
	}
	elsif ( $state == 1 && $line eq "{" ) {
		$state = 2;
	}
	elsif ( ( $state == 2 || $state == 3 ) && $line =~ m/}\s*;\s*$/ ) {
		print $output_file_cpp generate_converter( $enum_name, @enum_values );

		@enum_values = ();
		$state       = 0;
	}
	elsif ( $state == 2 && $line =~ m/([^,]+)\s*=\s*(\d+)\s*(,?)$/ ) {
		if ( $3 ne "," ) {
			$state = 3;
		}

		push( @enum_values, $1 );
	}
	elsif ( $state == 2 && $line =~ m/([^,]+)\s*(,?)$/ ) {
		if ( $2 ne "," ) {
			$state = 3;
		}

		push( @enum_values, $1 );
	}
}

