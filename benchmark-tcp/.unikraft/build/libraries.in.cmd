cmp -s /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libuklibid/libraries.in.new /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libuklibid/libraries.in; if [ $? -ne 0 ]; then cp /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libuklibid/libraries.in.new /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libuklibid/libraries.in; fi
