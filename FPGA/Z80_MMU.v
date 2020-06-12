// addr_get: 0xffbb -> ff -> a[ff] -> XX -> 0xXXbb
//data_forward: addr_get -> data, addr
// 0x01XX -> 01 -> a[XX] = data

//module addr_get: addr -> addr
//module addr_set: addr, data -> null
//module Z80_MMU: addr, data -> addr, data/null
module Z80_MMU  #(parameter SIZE = 256)(
					input wire[15:0] virtual_addr,
					input wire[7:0] data,
					output reg[15:0] physical_addr
				);

	reg[7:0] page_table[SIZE-1:0];
	wire [7:0] page_name;
	wire [7:0] offset;
	
	// setting initial page_table adresses to 0
	integer i;
	initial 
	begin
		for (i=0; i<SIZE; i=i+1) 
		begin
			page_table[i] = 0;
		end
	end

	// if virtual_addr is x00XX => setting XX page_table entry to data
	assign page_name = virtual_addr[15:8];
	assign offset = virtual_addr[7:0];
	always @(virtual_addr)
	begin
		if (page_name == 0) 
			page_table[offset][7:0] = data[7:0];
		else 
			physical_addr[15:0] = {page_table[page_name], virtual_addr[7:0]};
	end

	
endmodule


module z80_test ();
	reg [15:0] virtual_addr;
	reg [7:0] data;
	wire [15:0] physical_addr;
	
	wire [15:0] physical_addr_alias;
	parameter delay = 10;
	Z80_MMU mmu(virtual_addr, data, physical_addr);
	initial 
		begin
			#(delay) virtual_addr = 256'h00ac; data = 8'b10001110;
			#(delay) virtual_addr = 256'hacff; data = 8'b10001110;
		end
		

endmodule