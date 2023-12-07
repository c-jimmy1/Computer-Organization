// Adopted from https://circuitcove.com/design-examples-decoders/
`timescale 1 ns / 1 ps

module decoder
  # (parameter ENCODE_WIDTH = 4,
     parameter DECODE_WIDTH = 2**ENCODE_WIDTH
    )

  (
    input  [ENCODE_WIDTH-1:0] in,
    output [DECODE_WIDTH-1:0] out
  );

  localparam latency = 1;

  assign #latency out = 'b1<<in;

endmodule
