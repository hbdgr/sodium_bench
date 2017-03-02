import os

GNUPLOT_DATADIR="data_files"
OUTPUT_DIR="output"

def get_unique_functions(list_of_files):
    output = set()
    for file in list_of_files:
        function_name=file.partition("-")[0]
        output.add(function_name)
    return output

def get_unique_param(list_of_files, function):
    output = set()
    for file in list_of_files:
        if function + "-" in file:
            param_name = file.replace(".data", "").rsplit("-",maxsplit=1)[-1]
            output.add(param_name)
    return output

def write_gnuplot_header(gnuplot_file, fun_name, plot_num):
    gnuplot_file.write("set terminal pngcairo size 1200,1200 enhanced font 'Verdana,10'\n")
    gnuplot_file.write("set output '" + OUTPUT_DIR + "/" + fun_name + ".png'\n")
    gnuplot_file.write("set multiplot layout " + str(plot_num) + ", 1 title 'Threaded Authenticated Encryption' font ',16'\n")

def write_gnuplot_chart_title(gnuplot_file, title):
    gnuplot_file.write("set title '" + title + "'")
    gnuplot_file.write("set title  font ',12' norotate")

def write_gnuplot_x_axis(gnuplot_file, title, logscale=0):
    gnuplot_file.write("set xlabel '" + title + "'")
    if logscale:
        gnuplot_file.write("set logscale x " + str(logscale))

def write_gnuplot_y_axis(gnuplot_file, title, logscale=0):
    gnuplot_file.write("set ylabel '" + title + "'")
    if logscale:
        gnuplot_file.write("set logscale y " + str(logscale))

def write_gnuplot_xdata_axis(gnuplot_file):
    write_gnuplot_x_axis(gnuplot_file, "data size", 2)

    # specify tics according to data size
    gnuplot_file.write("set xtics ('1B' 1, '2B' 2,'4B' 4,'8B' 8,'16B' 16,'32B' 32,'64B' 64,'128B' 128,'256B' 256,'512B' 512,"
                        + "'1kB' 1024,'2kB' 2048,'4kB' 4096,'8kB' 8192,'16kB' 16384,'32kB' 32768,'64kB' 65536,'128kB' 131072,"
                        + "'256kB' 262144,'512kB' 524288,'1MB' 1048576, '2MB' 2097152, '4MB' 4194304, '8MB' 8388608,"
                        + "'16MB' 16777216, '32MB' 33554432 )\n")



if __name__ == "__main__":
    file_list = os.listdir(GNUPLOT_DATADIR)
    functions = get_unique_functions(file_list)

    for fun in functions:
        fun_param = get_unique_param(file_list, fun)
        fun_pretty_name = fun.replace("BM_","").replace("_","-").capitalize()
        gnuplot_file = open(fun_pretty_name.lower() + ".gnuplot", "w")

        write_gnuplot_header(gnuplot_file, fun_pretty_name, len(fun_param))
        write_gnuplot_xdata_axis(gnuplot_file)
        for param in fun_param:
            if param == "Mbps":
                print("Mbps")
            elif param == "latency":
                print("latency")
            elif param == "cpu_time":
                print("cpu_time")
            else:
                print("Parameter not recognized fun[" + fun  + "], param[" + param + "]" )

        gnuplot_file.close()
