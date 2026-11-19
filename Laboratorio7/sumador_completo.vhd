library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sumador_completo is
	Port ( A : in std_logic;
			 B : in std_logic;
			 Cin : in std_logic;
			 Cout : out std_logic;
			 S : out std_logic );
			 
end entity;

architecture Structural of sumador_completo is

    signal s_parcial : std_logic;
    signal c1        : std_logic;
    signal c2        : std_logic;
	 
begin 	
	
	MS1: entity work.medio_sumador
		  port map
		  (
				A => A,
				B => B,
				S => s_parcial,
				C => c1
		  );
		  
	MS2: entity work.medio_sumador
		  port map
		  (
				A => s_parcial,
				B => Cin,
				S => S,
				C => c2
		  );
	
	Cout <= c1 or c2;

end architecture; 