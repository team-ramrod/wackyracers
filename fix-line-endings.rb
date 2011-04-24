#!/usr/bin/env ruby

files = Dir['**/*.c'] + Dir['**/*.h'] + Dir['**/*.tex'] + Dir['**/*.txt'] + ['README', '.gitignore', 'Makefile']

files.map do |file_name|
  input = ''
  File.open(file_name, 'r') do |file|
    input = file.read
  end
  if /\r\n/ =~ input
    puts "Fixing file [#{file_name}]"
    File.open(file_name, 'w') do |file|
      file.write(input.gsub(/\r\n/, "\n"))
    end
  end
end
