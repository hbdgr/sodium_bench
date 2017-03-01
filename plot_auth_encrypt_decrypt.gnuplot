set terminal pngcairo size 1200,1200 enhanced font 'Verdana,10'
set output 'output/Auth_Encypt_Decrypt.png'

set multiplot layout 3, 1 title "Threaded Authenticated Encryption and Decryption" font ",16"

set title "Speed in Mbps"
set title  font ",12" norotate

set logscale x 2
set xlabel "data size"
set xtics (	'4B' 4,'8B' 8,'16B' 16,'32B' 32,'64B' 64,'128B' 128,'256B' 256,'512B' 512, \
			'1kB' 1024,'2kB' 2048,'4kB' 4096,'8kB' 8192,'16kB' 16384,'32kB' 32768,'64kB' 65536,'128kB' 131072,'256kB' 262144,'512kB' 524288, \
			'1MB' 1048576, '2MB' 2097152, '4MB' 4194304, '8MB' 8388608, '16MB' 16777216, '32MB' 33554432 )


set ylabel "speed rate [Mbps]"

plot 'data_files/BM_threaded_auth_encrypt_decrypt-1-Mbps.data' with lines title "threads: 1" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-2-Mbps.data' with lines title "threads: 2" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-4-Mbps.data' with lines title "threads: 4" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-8-Mbps.data' with lines title "threads: 8" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-12-Mbps.data' with lines title "threads: 12" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-16-Mbps.data' with lines title "threads: 16" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-32-Mbps.data' with lines title "threads: 32" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-48-Mbps.data' with lines title "threads: 48" axes x1y1


set title "Latency in nano seconds"
set title  font ",12" norotate

set ylabel "latency [ns]"
set logscale y 10

plot 'data_files/BM_threaded_auth_encrypt_decrypt-1-latency.data' with lines title "threads: 1" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-2-latency.data' with lines title "threads: 2" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-4-latency.data' with lines title "threads: 4" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-8-latency.data' with lines title "threads: 8" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-12-latency.data' with lines title "threads: 12" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-16-latency.data' with lines title "threads: 16" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-32-latency.data' with lines title "threads: 32" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-48-latency.data' with lines title "threads: 48" axes x1y1


set title "CPU time in nano seconds"
set title  font ",12" norotate

set ylabel "cpu time [ns]"
#set logscale y 10

plot 'data_files/BM_threaded_auth_encrypt_decrypt-1-cpu_time.data' with lines title "threads: 1" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-2-cpu_time.data' with lines title "threads: 2" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-4-cpu_time.data' with lines title "threads: 4" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-8-cpu_time.data' with lines title "threads: 8" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-12-cpu_time.data' with lines title "threads: 12" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-16-cpu_time.data' with lines title "threads: 16" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-32-cpu_time.data' with lines title "threads: 32" axes x1y1, \
	'data_files/BM_threaded_auth_encrypt_decrypt-48-cpu_time.data' with lines title "threads: 48" axes x1y1

#unset multiplot
#unset output
# pause 1
#reread
