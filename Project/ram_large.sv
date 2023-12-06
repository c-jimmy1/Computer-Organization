// Adopted from https://www.chipverify.com/verilog/verilog-single-port-ram
`include "ram.sv"
`include "decoder.sv"

`timescale 1 ns / 1 ps

module single_port_sync_ram_large
  # ( parameter ADDR_WIDTH = 14,
      parameter DATA_WIDTH = 16,
      parameter DATA_WIDTH_SHIFT = 1
    )
  
  (   input clk,
      input [ADDR_WIDTH-1:0] addr,
      inout [DATA_WIDTH-1:0] data,
      input cs_input,
      input we,
      input oe
  );
  
  wire [3:0] cs;
  
  decoder #(.ENCODE_WIDTH(2)) dec
  (   .in(addr[ADDR_WIDTH-1:ADDR_WIDTH-2]),
      .out(cs) 
  );
  
  single_port_sync_ram  #(.DATA_WIDTH(DATA_WIDTH/2)) u00
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[(DATA_WIDTH>>DATA_WIDTH_SHIFT)-1:0]),
      .cs(cs[0]),
      .we(we),
      .oe(oe)
  );
  single_port_sync_ram #(.DATA_WIDTH(DATA_WIDTH>>DATA_WIDTH_SHIFT)) u01
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[DATA_WIDTH-1:DATA_WIDTH>>DATA_WIDTH_SHIFT]),
      .cs(cs[0]),
      .we(we),
      .oe(oe)
  );

  single_port_sync_ram  #(.DATA_WIDTH(DATA_WIDTH/2)) u10
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[(DATA_WIDTH>>DATA_WIDTH_SHIFT)-1:0]),
      .cs(cs[1]),
      .we(we),
      .oe(oe)
  );
  single_port_sync_ram #(.DATA_WIDTH(DATA_WIDTH>>DATA_WIDTH_SHIFT)) u11
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[DATA_WIDTH-1:DATA_WIDTH>>DATA_WIDTH_SHIFT]),
      .cs(cs[1]),
      .we(we),
      .oe(oe)
  );

  single_port_sync_ram  #(.DATA_WIDTH(DATA_WIDTH/2)) u20
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[(DATA_WIDTH>>DATA_WIDTH_SHIFT)-1:0]),
      .cs(cs[2]),
      .we(we),
      .oe(oe)
  );
  single_port_sync_ram #(.DATA_WIDTH(DATA_WIDTH>>DATA_WIDTH_SHIFT)) u21
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[DATA_WIDTH-1:DATA_WIDTH>>DATA_WIDTH_SHIFT]),
      .cs(cs[2]),
      .we(we),
      .oe(oe)
  );

  single_port_sync_ram  #(.DATA_WIDTH(DATA_WIDTH/2)) u30
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[(DATA_WIDTH>>DATA_WIDTH_SHIFT)-1:0]),
      .cs(cs[3]),
      .we(we),
      .oe(oe)
  );
  single_port_sync_ram #(.DATA_WIDTH(DATA_WIDTH>>DATA_WIDTH_SHIFT)) u31
  (   .clk(clk),
      .addr(addr[ADDR_WIDTH-3:0]),
      .data(data[DATA_WIDTH-1:DATA_WIDTH>>DATA_WIDTH_SHIFT]),
      .cs(cs[3]),
      .we(we),
      .oe(oe)
  );

endmodule
