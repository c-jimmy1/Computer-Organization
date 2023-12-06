// Adopted from https://www.chipverify.com/verilog/verilog-single-port-ram
`timescale 1 ns / 1 ps

module test_ram;
  parameter ADDR_WIDTH = 14;
  parameter DATA_WIDTH = 16;

  reg clk;
  reg cs;
  reg we;
  reg oe;
  reg [ADDR_WIDTH-1:0] addr;
  wire [DATA_WIDTH-1:0] data;
  reg [DATA_WIDTH-1:0] testbench_data;

  single_port_sync_ram_large  #(.DATA_WIDTH(DATA_WIDTH)) u0
  (   .clk(clk),
      .addr(addr),
      .data(data[DATA_WIDTH-1:0]),
      .cs_input(cs),
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

    // Write
    for (i = 2**(ADDR_WIDTH-2)-4; i < 2**(ADDR_WIDTH-2); i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 1; cs <= 1; oe <= 0; testbench_data <= $random;
    end

    for (i = 2**(ADDR_WIDTH-1)-4; i < 2**(ADDR_WIDTH-1); i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 1; cs <= 1; oe <= 0; testbench_data <= $random;
    end

    for (i = 2**(ADDR_WIDTH-1)+2**(ADDR_WIDTH-2)-4; i < 2**(ADDR_WIDTH-1)+2**(ADDR_WIDTH-2); i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 1; cs <= 1; oe <= 0; testbench_data <= $random;
    end

    for (i = 2**ADDR_WIDTH-4; i < 2**ADDR_WIDTH-4; i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 1; cs <= 1; oe <= 0; testbench_data <= $random;
    end

    // Read
    for (i = 2**(ADDR_WIDTH-2)-4; i < 2**(ADDR_WIDTH-2); i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 0; cs <= 1; oe <= 1;
    end

    for (i = 2**(ADDR_WIDTH-1)-4; i < 2**(ADDR_WIDTH-1); i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 0; cs <= 1; oe <= 1;
    end

    for (i = 2**(ADDR_WIDTH-1)+2**(ADDR_WIDTH-2)-4; i < 2**(ADDR_WIDTH-1)+2**(ADDR_WIDTH-2); i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 0; cs <= 1; oe <= 1;
    end

    for (i = 2**ADDR_WIDTH-4; i < 2**ADDR_WIDTH-4; i = i+1) begin
      repeat (1) @(posedge clk) addr <= i; we <= 0; cs <= 1; oe <= 1;
    end
    
    #40 $finish;
  end
endmodule
