library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_medio_sumador is
end entity;

architecture Behavioral of tb_medio_sumador is

    signal A_tb : std_logic := '0';
    signal B_tb : std_logic := '0';
    signal S_tb : std_logic;
    signal C_tb : std_logic;
	 
begin

	uut: entity work.medio_sumador
		  port map(
						A => A_tb,
						B => B_tb,
						S => S_tb,
						C => C_tb  );
	
	stim_proc: process
	begin
		  -- Caso 1: A = 0, B = 0 -> Esperado: S = 0, C = 0
        A_tb <= '0'; B_tb <= '0';
        wait for 10 ns;
        assert (S_tb = '0')
            report "Fallo en salida S | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido S=" & std_logic'image(S_tb) & " | Esperado S='0'"
            severity error;
        assert (C_tb = '0')
            report "Fallo en salida C | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido C=" & std_logic'image(C_tb) & " | Esperado C='0'"
            severity error;

        -- Caso 2: A = 0, B = 1 -> Esperado: S = 1, C = 0
        A_tb <= '0'; B_tb <= '1';
        wait for 10 ns;
        assert (S_tb = '1')
            report "Fallo en salida S | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido S=" & std_logic'image(S_tb) & " | Esperado S='1'"
            severity error;
        assert (C_tb = '0')
            report "Fallo en salida C | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido C=" & std_logic'image(C_tb) & " | Esperado C='0'"
            severity error;

        -- Caso 3: A = 1, B = 0 -> Esperado: S = 1, C = 0
        A_tb <= '1'; B_tb <= '0';
        wait for 10 ns;
        assert (S_tb = '1')
            report "Fallo en salida S | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido S=" & std_logic'image(S_tb) & " | Esperado S='1'"
            severity error;
        assert (C_tb = '0')
            report "Fallo en salida C | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido C=" & std_logic'image(C_tb) & " | Esperado C='0'"
            severity error;

        -- Caso 4: A = 1, B = 1 -> Esperado: S = 0, C = 1
        A_tb <= '1'; B_tb <= '1';
        wait for 10 ns;
        assert (S_tb = '0')
            report "Fallo en salida S | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido S=" & std_logic'image(S_tb) & " | Esperado S='0'"
            severity error;
        assert (C_tb = '1')
            report "Fallo en salida C | Entradas: A=" & std_logic'image(A_tb) & " B=" & std_logic'image(B_tb) &
                   " | Obtenido C=" & std_logic'image(C_tb) & " | Esperado C='1'"
            severity error;

        -- Mensaje de confirmación final
        report "Simulacion finalizada correctamente: los 4 casos pasaron sin errores." severity note;
        wait; -- Detiene el proceso indefinidamente para que no vuelva a empezar
	end process;
end architecture;
	