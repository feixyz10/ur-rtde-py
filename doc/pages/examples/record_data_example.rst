.. _record-data-example:

Record Data Example
===================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example shows how to record the robot data to a (.csv) file of your choice. You
can set the frequency at which the data is recorded. Optionally, you can decide to only
record a subset of the available robot data. You do this by passing a std::vector<std::string> to
startFileRecording() that contains the names of the variables that you want to record. eg.

.. code-block:: c++

   ...
   std::vector<std::string> record_variables = {"timestamp", "actual_q", "actual_TCP_pose"};
   rtde_receive.startFileRecording("robot_data.csv", record_variables);
   ...

by default all variables are recorded, and you are not required to pass the variables argument.

C++:

.. code-block:: c++

   #include <ur_rtde/rtde_receive_interface.h>
   #include <boost/program_options.hpp>
   #include <thread>
   #include <chrono>
   #include <csignal>
   #include <string>
   #include <iostream>

   using namespace ur_rtde;
   using namespace std::chrono;
   namespace po = boost::program_options;

   // Interrupt flag
   bool flag_loop = true;
   void raiseFlag(int param)
   {
     flag_loop = false;
   }

   int main(int argc, char* argv[])
   {
     try {
       po::options_description desc("Allowed options");
       desc.add_options()
           ("help", "Record robot data to a (.csv) file")
           ("robot_ip", po::value<std::string>()->default_value("localhost"),
                "the IP address of the robot")
           ("frequency", po::value<double>()->default_value(500.0),
                    "the frequency at which the data is recorded (default is 500Hz)")
           ("output", po::value<std::string>()->default_value("robot_data.csv"),
                        "data output (.csv) file to write to (default is \"robot_data.csv\"")
           ;

       po::variables_map vm;
       po::store(po::parse_command_line(argc, argv, desc), vm);
       po::notify(vm);

       if (vm.count("help")) {
         std::cout << desc << "\n";
         return 0;
       }

       signal(SIGINT, raiseFlag);
       double frequency = vm["frequency"].as<double>();
       double dt = 1.0 / frequency;
       RTDEReceiveInterface rtde_receive(vm["robot_ip"].as<std::string>(), frequency);

       rtde_receive.startFileRecording(vm["output"].as<std::string>());
       std::cout << "Data recording started. press [Ctrl-C] to end recording." << std::endl;
       int i=0;
       while (flag_loop)
       {
         auto t_start = steady_clock::now();
         if (i % 10 == 0)
         {
           std::cout << '\r';
           printf("%.3d samples.", i);
           std::cout << std::flush;
         }
         auto t_stop = steady_clock::now();
         auto t_duration = std::chrono::duration<double>(t_stop - t_start);
         if (t_duration.count() < dt)
         {
           std::this_thread::sleep_for(std::chrono::duration<double>(dt - t_duration.count()));
         }
         i++;
       }

       // Stop data recording.
       rtde_receive.stopFileRecording();
       std::cout << "\nData recording stopped." << std::endl;
     }
     catch(std::exception& e) {
       std::cerr << "error: " << e.what() << "\n";
       return 1;
     }
     catch(...) {
       std::cerr << "Exception of unknown type!\n";
     }
     return 0;
   }

Python:

.. code-block:: python

   from rtde_receive import RTDEReceiveInterface as RTDEReceive
   import time
   import argparse
   import sys


   def parse_args(args):
       """Parse command line parameters

       Args:
         args ([str]): command line parameters as list of strings

       Returns:
         :obj:`argparse.Namespace`: command line parameters namespace
       """
       parser = argparse.ArgumentParser(
           description="Record data example")
       parser.add_argument(
           "-ip",
           "--robot_ip",
           dest="ip",
           help="IP address of the UR robot",
           type=str,
           default='localhost',
           metavar="<IP address of the UR robot>")
       parser.add_argument(
           "-o",
           "--output",
           dest="output",
           help="data output (.csv) file to write to (default is \"robot_data.csv\"",
           type=str,
           default="robot_data.csv",
           metavar="<data output file>")
       parser.add_argument(
           "-f",
           "--frequency",
           dest="frequency",
           help="the frequency at which the data is recorded (default is 500Hz)",
           type=float,
           default=500.0,
           metavar="<frequency>")

       return parser.parse_args(args)


   def main(args):
       """Main entry point allowing external calls

       Args:
         args ([str]): command line parameter list
       """
       args = parse_args(args)
       dt = 1 / args.frequency
       rtde_r = RTDEReceive(args.ip, args.frequency)
       rtde_r.startFileRecording(args.output)
       print("Data recording started, press [Ctrl-C] to end recording.")
       i = 0
       try:
           while True:
               start = time.time()
               if i % 10 == 0:
                   sys.stdout.write("\r")
                   sys.stdout.write("{:3d} samples.".format(i))
                   sys.stdout.flush()
               end = time.time()
               duration = end - start

               if duration < dt:
                   time.sleep(dt - duration)
               i += 1

       except KeyboardInterrupt:
           rtde_r.stopFileRecording()
           print("\nData recording stopped.")


   if __name__ == "__main__":
       main(sys.argv[1:])

You can find the source code of this example under :file:`examples/cpp/record_data_example.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 record_data_example.py`.

.. _move-asynchronous-example:

