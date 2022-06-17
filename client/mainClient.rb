require 'socket'

s=  TCPSocket.new 'localhost' , 18000

s.write("\n")

s.each_line do |line|
    puts line
end

s.close