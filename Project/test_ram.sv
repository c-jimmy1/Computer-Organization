// Adopted from https://www.chipverify.com/verilog/verilog-single-port-ram
`timescale 1 ns / 1 ps

module test_ram;
  parameter ADDR_WIDTH = 12;
  parameter DATA_WIDTH = 8;

  reg clk;
  reg cs;
  reg we;
  reg oe;
  reg [ADDR_WIDTH-1:0] addr;
  wire [DATA_WIDTH-1:0] data;
  reg [DATA_WIDTH-1:0] testbench_data;

  single_port_sync_ram #(.DATA_WIDTH(DATA_WIDTH)) u0
  (   .clk(clk),
      .addr(addr),
      .data(data),
      .cs(cs),
      .we(we),
      .oe(oe)
  );

  always #20 clk = ~clk;
  assign data = !oe ? testbench_data : 'hz;

  integer i;
  initial begin
    $dumpfile("dump.vcd");
    $dumpvars;
    {clk, cs, we, addr, testbench_data, oe} <= 0;

    repeat (2) @ (posedge clk);

    for (i = 0; i < 16; i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 1; cs <= 1; oe <= 0; testbench_data <= $random;
    end

    for (i = 0; i < 16; i= i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 0; cs <= 1; oe <= 1;
    end

    @(posedge clk) cs <= 0;

    #360 $finish;
  end
endmodule
