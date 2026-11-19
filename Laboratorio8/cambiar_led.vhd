library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity cambiar_led is

    port (
	     reset : in  std_logic;
        clk   : in  std_logic;
        key0  : in  std_logic;
		  key1  : in  std_logic;  
        led   : out std_logic  
    );
end entity;

architecture structural of cambiar_led is
	signal key1_inv   : std_logic;
   signal key1_sync  : std_logic;
   signal pulso_val  : std_logic;
   signal reset_high : std_logic;
   signal led_reg    : std_logic := '0';
	
begin
	
	key1_inv    <= not key1;
	reset_high <= not key0;
	
	u_sync: entity work.sincronizador_2ff
		port map
		(
			clk           => clk,
			entrada_async => key1_inv,
			salida_sync   => key1_sync
		);
	u_det: entity work.detector_flanco
		port map
		(
			clk           => clk,  
         reset         => reset_high,
         entrada_sync  => key1_sync,
		   pulso 		  => pulso_val
		);
	
	process(clk)
	begin
		if rising_edge(clk) then
			if reset_high = '1' then
                led_reg <= '0';
         elsif pulso_val = '1' then
                led_reg <= not led_reg;
         end if;
      end if;
	end process;
	led <= led_reg;
end architecture;
