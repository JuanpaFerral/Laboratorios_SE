library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sumador_n is
	generic
	(
		N : positive := 4
	);
	Port ( 
			 A : in std_logic_vector(N - 1 downto 0);
			 B : in std_logic_vector(N - 1 downto 0);
			 Cin : in std_logic;
			 Cout : out std_logic;
			 S : out std_logic_vector(N - 1 downto 0) 
			);
			 
end entity;

architecture Structural of sumador_n is

    signal carry : std_logic_vector(N downto 0);
	 
begin
	
	carry(0) <=  Cin;
	Cout     <=  carry(N);
	
	gen_sumadores : for i in 0 to N-1 generate
		
		FA_i: entity work.sumador_completo
					port map
					(
						A    => A(i),
						B    => B(i),
						Cin  => carry(i),
						S    => S(i),
						Cout => carry(i+1)
					);
		end generate;
end architecture;