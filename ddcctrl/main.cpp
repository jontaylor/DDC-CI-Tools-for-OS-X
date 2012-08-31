#include <iostream>
#include <sstream>
#include <string>
#include "ddc.h"
#include "getopt_pp.h"

using namespace GetOpt;

void display_help();

int main(int argc, char* argv[]) {
	
	bool help_flag;
	bool set_flag;
	bool get_flag;
	std::string new_value;
	std::string display;
	std::string control;
    int new_value_int;
	int display_int;
	int control_int;
	
	
	bool list_displays;
	
	try
	{	
		GetOpt_pp ops(argc, argv);
		
		ops.exceptions(std::ios::failbit);
		
		ops 
		>> OptionPresent('h', "help", help_flag)
		>> OptionPresent('s', "set", set_flag)
		>> OptionPresent('g', "get", get_flag)
		
		>> OptionPresent('l', "listdisplays", list_displays)
		
		>> Option('c', "control", control)
		>> Option('v', "value", new_value, "0")
		>> Option('d', "display", display, "0")
		;
		
		
		if (!ops.options_remain())
		{
			if ((help_flag) or ((set_flag+get_flag+list_displays) == 0) ) {
			    display_help();	
				return 0;
			}
			
			if ((set_flag + get_flag +  list_displays) != 1) {
				std::cerr << "Please specify only one of the following\n";
				std::cerr << "set | get | listcontrols | listdisplays \n";
	            return 1;
			}

			
			if (sscanf(new_value.c_str(), "%d", &new_value_int) == EOF) { std::cerr << "Value needs to be an integer\n"; return 1; }
			if (sscanf(display.c_str(), "%d", &display_int) == EOF) { std::cerr << "Display needs to be an integer\n"; return 1; }
						
			if (set_flag) {
				if (!(ops >>OptionPresent('c',"control")) || !(ops >> OptionPresent('v',"value")) || !(ops >> OptionPresent('d',"display"))) {
					std::cerr << "Control,Value and Display need to be specified\n";
					return 1;
				}
				
			    int result;
				struct DDCWriteCommand write_command;
				write_command.control_id = atoi(control.c_str());
				write_command.new_value = new_value_int;
				result = ddc_write(display_int, &write_command);
				return !result;
			}
			
			if (get_flag) {
				if (!(ops >>OptionPresent('c',"control"))  || !(ops >> OptionPresent('d',"display"))) {
					std::cerr << "Control and Display need to be specified\n";
					return 1;
				}
				
				int result;
				struct DDCReadCommand read_command;
				read_command.control_id = atoi(control.c_str());
				
				result = ddc_read(display_int, &read_command);
				
				std::cout << "Current Value: " << (int)read_command.response.current_value << "\n";
				std::cout << "Maximum Value: " << (int)read_command.response.max_value << "\n";
				
			}
			
			
			return 0;
		}
		else
		{
			std::cerr << "too many options" << std::endl;
			return 1;
		}
	}
	catch(const GetOptEx& e)
	{
		std::cerr << "Invalid options\n";
		std::cerr << "control should be a string\n";
		std::cerr << "value and display should be integers\n";
		display_help();
	}    
	
	return 0;
}


void display_help() {
	std::cout << "Options are blah\n";
}


/*
int main (int argc, char * const argv[]) {
    // insert code here...
    //std::cout << "Hello, World!\n";
	
	int i;
	
	//std::cout << primary_display_id();
	//printf("<#message#>");
	struct DDCWriteCommand write_command;
	
	
	struct DDCReadCommand read_command;
	//p_write_command = &write_command;
	
	
	write_command.control_id = 0x60;
	write_command.new_value = 0x05;
	//write_command.new_value = 0x01;
	
	//printf("<#message#>");
	i = ddc_write(1, &write_command);

	
	//read_command.control_id = 0x10;
	
	//i = ddc_read(1, &read_command); 
	
	//edid_test(0);
	
	//printf("\nend\n");
	
	//printf(read_command.reply_buffer);
	
    //IOI2CConnectRef connection;
	
	//connection = display_connection(0);
	//IOI2CInterfaceClose( connection, kNilOptions );
	
    return 0;
}
 
 */
