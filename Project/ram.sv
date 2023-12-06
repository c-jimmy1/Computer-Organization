// Adopted from https://www.chipverify.com/verilog/verilog-single-port-ram
`timescale 1 ns / 1 ps

module single_port_sync_ram
  # (parameter ADDR_WIDTH = 12,
     parameter DATA_WIDTH = 8,
     parameter LENGTH = (1<<ADDR_WIDTH)
    )

  (   input clk,
      input [ADDR_WIDTH-1:0] addr,
      inout [DATA_WIDTH-1:0] data,
      input cs,
      input we,
      input oe
  );

  reg [DATA_WIDTH-1:0] tmp_data;
  reg [DATA_WIDTH-1:0] mem[LENGTH];

  always @ (posedge clk) begin
    // Used when debugging a multiplication program
    // if (addr=='h10D) $display("Product is %d", mem[addr]);
    if (cs & we)
      mem[addr] <= data;
  end
  
  always @ (negedge clk) begin
    if (cs & !we)
      tmp_data <= mem[addr];
  end

  assign data = cs & oe & !we ? tmp_data : 'hz;
endmodule
