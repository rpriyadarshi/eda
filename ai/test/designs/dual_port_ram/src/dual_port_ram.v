// Dual Port RAM Verilog Implementation

module dual_port_ram (
    input wire clk,
    input wire rst,
    input wire [9:0] addr_a,
    input wire [9:0] addr_b,
    input wire [15:0] data_a_write,
    input wire [15:0] data_b_write,
    input wire we_a,
    input wire we_b,
    output reg [15:0] data_a_read,
    output reg [15:0] data_b_read
);

reg [15:0] mem [1023:0]; // 1024 words of 16 bits each

always @(posedge clk) begin
    if (rst) begin
        data_a_read <= 16'b0;
        data_b_read <= 16'b0;
    end else begin
        if (we_a)
            mem[addr_a] <= data_a_write;
        if (we_b)
            mem[addr_b] <= data_b_write;
        data_a_read <= mem[addr_a];
        data_b_read <= mem[addr_b];
    end
end

endmodule
