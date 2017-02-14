#!/usr/bin/env bash


readonly FILENAME="results.txt"
readonly PROGNAME="sodium_bench"

function run_benchmark() {
	local program_name=$1
	local results_filename=$2
	./${program_name} | tee "${results_filename}"
}

# function arguments:
# $1 time in nanoseconds
function to_seconds() {
	local nanoseconds=$1
	local nanosecond_multiplier=1000000000

	#seconds
	local seconds=`echo "scale=10; ${nanoseconds}/${nanosecond_multiplier}" | bc -l`
	printf "${seconds}"
}

# function arguments:
# $1 size in bytes, kilobytes or megabytes
function to_bytes() {
	local human_bytes=$1
	local bytes_operation=`echo "$human_bytes" | sed -e 's/k/*1024/g' | sed -e 's/M/*1024*1024/g'`

	#bytes
	local bytes=`echo "${bytes_operation}" | bc -l`
	printf "${bytes}"
}

# function arguments:
# $1 size in bytes, kilobytes or megabytes
# $2 time in nanoseconds
function calc_Mbps() {
	local bytes=`to_bytes "$1"`
	local seconds=`to_seconds "$2"`
	local Megabit_multiplier=`echo "1024*1024/8" | bc -l`

	local Mbps=`echo "${bytes} ${seconds} ${Megabit_multiplier}" | awk  '{ print ($1/$3)/$2 }' `
	printf "${Mbps}"
}

function read_result() {
	printf "Start reading / Analyze\n"

	while read line; do
		# results that we analyze should have name with "BM"
		if [[ $line != *"BM"* ]]; then
			continue
		fi

		local size=`echo $line | awk -F'/' '{ print $2 }' | awk '{ print $1 }'`
		local time=`echo $line | awk -F'/' '{ print $2 }' | awk '{ print $2 }'`

		local Mbps=`calc_Mbps "${size}" "${time}"`
		printf "${line} | "${Mbps}" Mbps\n"

	done < "${FILENAME}"
}

usage() {
cat <<- EOF
	usage: $PROGNAME options

	Program run benchmarks and analyze it

	OPTIONS:
	    -r --run-bench           run ${PROGNAME} binary with google benchmarks and save results to ${FILENAME}
	    -a --analyze             analyze  benchmark results from ${PROGNAME}
	    -h --help                show this help
EOF
}

cmdline() {
	# got this idea from here:
	# http://kirk.webfinish.com/2009/10/bash-shell-script-to-use-getopts-with-gnu-style-long-positional-parameters/
	local arg=$@
	for arg; do
		local delim=""
		case "$arg" in
			#translate --gnu-long-options to -g (short options)
			--run-bench)            args="${args}-r ";;
			--analyze)              args="${args}-a ";;
			--help)                 args="${args}-h ";;
			#pass through anything else
			*) [[ "${arg:0:1}" == "-" ]] || delim="\""
				args="${args}${delim}${arg}${delim} ";;
		esac
	done

	#Reset the positional parameters to the short options
	eval set -- $args

	while getopts "hra" OPTION; do
		case $OPTION in
		r)
			set -o errexit
			set -o nounset
			run_benchmark "${PROGNAME}" "${FILENAME}"
			;;
		a)
			set -o errexit
			set -o nounset
			read_result
			;;
		h)
			usage
			exit 0
			;;
		esac
	done

	return 0
}

main() {
	cmdline $@
}
main $@
