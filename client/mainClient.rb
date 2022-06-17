require 'socket'


startTime =  Process.clock_gettime(Process::CLOCK_MONOTONIC)

s=  TCPSocket.new 'localhost' , 18000

s.write("\n")

s.each_line do |line|
    #puts line
end

s.close

endtime =  Process.clock_gettime(Process::CLOCK_MONOTONIC)

elapsed = endtime - startTime

puts "Elapsed time: #{elapsed}"