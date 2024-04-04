create_clock -period 10 -name clk [get_ports clk]

# Set input delay for address and data inputs
set_input_delay -clock clk 0 [get_ports {addr_a addr_b data_a_write data_b_write}]
set_output_delay -clock clk 0 [get_ports {data_a_read data_b_read}]

# Set max delay for the RAM read output
set_max_delay 2 -from [get_ports {addr_a addr_b}] -to [get_ports {data_a_read data_b_read}]

# Set max delay for the RAM write input
set_max_delay 2 -from [get_ports {addr_a addr_b data_a_write data_b_write}] -to [get_registers *]

# Set false path between address and data ports
set_false_path -from [get_ports {addr_a addr_b}] -to [get_ports {data_a_write data_b_write}]

# Set max delay for the reset input
set_max_delay 3 -from [get_ports rst] -to [get_registers *]
