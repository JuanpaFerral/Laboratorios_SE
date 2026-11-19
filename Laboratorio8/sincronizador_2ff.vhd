library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Sincronizador de una entrada asincrona mediante dos flip-flops.
-- Reduce la probabilidad de propagacion de metastabilidad
-- al incorporar una senal externa al dominio de reloj.

entity sincronizador_2ff is
    port (
        clk           : in  std_logic;  -- Reloj del sistema
        entrada_async : in  std_logic;  -- Entrada asincrona
        salida_sync   : out std_logic   -- Salida sincronizada
    );
end entity sincronizador_2ff;

architecture rtl of sincronizador_2ff is

    -- Registros de las dos etapas de sincronizacion.
    signal etapa1 : std_logic;
    signal etapa2 : std_logic;

begin

    process(clk)
    begin
        if rising_edge(clk) then
            -- Primera etapa: captura la entrada asincrona.
            etapa1 <= entrada_async;

            -- Segunda etapa: captura el valor anterior de etapa1.
            -- Reduce la probabilidad de que una posible metastabilidad
            -- de la primera etapa se propague al resto del circuito.
            etapa2 <= etapa1;
        end if;
    end process;

    -- La salida se obtiene de la segunda etapa.
    salida_sync <= etapa2;

end architecture rtl;