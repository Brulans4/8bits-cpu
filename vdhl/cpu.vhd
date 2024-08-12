library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


-- 8-bit register
entity Register8 is
    Port (
        clk     : in  STD_LOGIC;
        rst     : in  STD_LOGIC;
        ld      : in  STD_LOGIC;
        d_in    : in  STD_LOGIC_VECTOR(7 downto 0);
        q_out   : out STD_LOGIC_VECTOR(7 downto 0)
    );
end Register8;

architecture Behavioral of Register8 is
    signal reg : STD_LOGIC_VECTOR(7 downto 0);
begin
    process(clk, rst)
    begin
        if rst = '1' then
            reg <= (others => '0');  -- Clear register on reset
        elsif rising_edge(clk) then
            if ld = '1' then
                reg <= d_in;  -- Load new data into the register
            end if;
        end if;
    end process;
    
    q_out <= reg;  -- Output the value of the register
end Behavioral;


-- 8-bit RAM
entity RAM256 is
    Port (
        clk      : in  STD_LOGIC;
        we       : in  STD_LOGIC;
        addr     : in  STD_LOGIC_VECTOR(7 downto 0);
        data_in  : in  STD_LOGIC_VECTOR(7 downto 0);
        data_out : out STD_LOGIC_VECTOR(7 downto 0)
    );
end RAM256;

architecture Behavioral of RAM256 is
    type RAM_TYPE is array (255 downto 0) of STD_LOGIC_VECTOR(7 downto 0);
    signal ram : RAM_TYPE := (others => (others => '0'));
begin
    process(clk)
    begin
        if rising_edge(clk) then
            if we = '1' then
                ram(to_integer(unsigned(addr))) <= data_in;  -- Write data to RAM
            end if;
            data_out <= ram(to_integer(unsigned(addr)));  -- Read data from RAM
        end if;
    end process;
end Behavioral;


-- 8-bit ALU
entity ALU8 is
    Port (
        A        : in  STD_LOGIC_VECTOR(7 downto 0);
        B        : in  STD_LOGIC_VECTOR(7 downto 0);
        opcode   : in  STD_LOGIC_VECTOR(3 downto 0);
        result   : out STD_LOGIC_VECTOR(7 downto 0);
        carry    : out STD_LOGIC;
        zero     : out STD_LOGIC
    );
end ALU8;

architecture Behavioral of ALU8 is
    signal tmp_result : STD_LOGIC_VECTOR(8 downto 0);
begin
    process(A, B, opcode)
    begin
        carry <= '0';
        zero <= '0';
        case opcode is
            when "0001" =>  -- MOV (just pass A through)
                tmp_result <= "0" & A;
            when "0100" =>  -- INC (A + 1)
                tmp_result <= ('0' & A) + 1;
            when "0101" =>  -- ADD (A + B)
                tmp_result <= ('0' & A) + ('0' & B);
            when "0110" =>  -- ADDIV (A + Immediate)
                tmp_result <= ('0' & A) + ('0' & B);  -- Assuming B holds the immediate value
            when "0111" =>  -- DEC (A - 1)
                tmp_result <= ('0' & A) - 1;
            when "1000" =>  -- SUB (A - B)
                tmp_result <= ('0' & A) - ('0' & B);
            when "1001" =>  -- SUBIV (A - Immediate)
                tmp_result <= ('0' & A) - ('0' & B);  -- Assuming B holds the immediate value
            when "1010" =>  -- OR (A | B)
                tmp_result <= "0" & (A or B);
            when "1011" =>  -- AND (A & B)
                tmp_result <= "0" & (A and B);
            when "1100" =>  -- CMP (A - B)
                tmp_result <= ('0' & A) - ('0' & B);
            when others =>  -- Default case (NOP or unsupported opcodes)
                tmp_result <= (others => '0');
        end case;

        result <= tmp_result(7 downto 0);  -- Assign the lower 8 bits to the result
        carry <= tmp_result(8);            -- Carry flag (9th bit of the result)
        zero <= '1' when result = "00000000" else '0';  -- Zero flag
    end process;
end Behavioral;


-- 8-bit CU
entity ControlUnit is
    Port (
        clk              : in  STD_LOGIC;
        reset            : in  STD_LOGIC;
        opcode           : in  STD_LOGIC_VECTOR(3 downto 0);
        carry_flag       : in  STD_LOGIC;
        zero_flag        : in  STD_LOGIC;
        pc_current       : in  STD_LOGIC_VECTOR(7 downto 0);
        mem_address      : out STD_LOGIC_VECTOR(7 downto 0);
        reg_write_enable : out STD_LOGIC;
        mem_read_enable  : out STD_LOGIC;
        mem_write_enable : out STD_LOGIC;
        alu_op           : out STD_LOGIC_VECTOR(3 downto 0);
        pc_update        : out STD_LOGIC_VECTOR(7 downto 0)
    );
end ControlUnit;

architecture Behavioral of ControlUnit is
    signal next_pc      : STD_LOGIC_VECTOR(7 downto 0);
    signal alu_op_signal: STD_LOGIC_VECTOR(3 downto 0);
begin

    process(clk, reset)
    begin
        if reset = '1' then
            reg_write_enable <= '0';
            mem_read_enable  <= '0';
            mem_write_enable <= '0';
            alu_op           <= "0000";
            pc_update        <= (others => '0');
        elsif rising_edge(clk) then
            case opcode is
                -- NOP
                when "0000" =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    next_pc <= pc_current + 1;
                    
                -- MOV
                when "0001" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    next_pc <= pc_current + 1;
                    
                -- STOR
                when "0010" =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '1';
                    alu_op           <= "0000";
                    next_pc <= pc_current + 2;
                    
                -- LOAD
                when "0011" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '1';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    next_pc <= pc_current + 2;
                    
                -- INC
                when "0100" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0100";  -- Specify INC operation
                    next_pc <= pc_current + 1;
                    
                -- ADD
                when "0101" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0101";  -- Specify ADD operation
                    next_pc <= pc_current + 1;
                    
                -- ADDIV
                when "0110" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0110";  -- Specify ADDIV operation
                    next_pc <= pc_current + 2;
                    
                -- DEC
                when "0111" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0111";  -- Specify DEC operation
                    next_pc <= pc_current + 1;
                    
                -- SUB
                when "1000" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "1000";  -- Specify SUB operation
                    next_pc <= pc_current + 1;
                    
                -- SUBIV
                when "1001" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "1001";  -- Specify SUBIV operation
                    next_pc <= pc_current + 2;
                    
                -- OR
                when "1010" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "1010";  -- Specify OR operation
                    next_pc <= pc_current + 1;
                    
                -- AND
                when "1011" =>
                    reg_write_enable <= '1';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "1011";  -- Specify AND operation
                    next_pc <= pc_current + 1;
                    
                -- CMP
                when "1100" =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "1100";  -- Specify CMP operation
                    next_pc <= pc_current + 1;
                    
                -- JMP
                when "1101" =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    next_pc <= RAM_address;  -- RAM_address would be the jump target
                
                -- FLG
                when "1110" =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    if carry_flag = '1' or zero_flag = '0' then
                        next_pc <= RAM_address;  -- RAM_address would be the jump target
                    else
                        next_pc <= pc_current + 2;
                    end if;
                    
                -- RSTFG
                when "1111" =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    carry_flag <= '0';
                    zero_flag  <= '0';
                    next_pc <= pc_current + 1;
                    
                when others =>
                    reg_write_enable <= '0';
                    mem_read_enable  <= '0';
                    mem_write_enable <= '0';
                    alu_op           <= "0000";
                    next_pc <= pc_current + 1;
            end case;
        end if;
    end process;

    -- Output assignments
    pc_update <= next_pc;
end Behavioral;


-- 8-bit CPU
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity CPU is
    Port (
        clk         : in  STD_LOGIC;
        reset       : in  STD_LOGIC;
        -- External connections (e.g., for debugging or I/O)
        mem_address : out STD_LOGIC_VECTOR(7 downto 0);
        mem_data_in : in  STD_LOGIC_VECTOR(7 downto 0);
        mem_data_out: out STD_LOGIC_VECTOR(7 downto 0)
    );
end CPU;

architecture Behavioral of CPU is
    -- Component declarations
    component RegisterFile is
        Port (
            clk        : in  STD_LOGIC;
            reset      : in  STD_LOGIC;
            reg_write  : in  STD_LOGIC;
            reg_data   : in  STD_LOGIC_VECTOR(7 downto 0);
            reg_addr   : in  STD_LOGIC_VECTOR(1 downto 0);
            reg_out    : out STD_LOGIC_VECTOR(7 downto 0)
        );
    end component;

    component ALU is
        Port (
            a          : in  STD_LOGIC_VECTOR(7 downto 0);
            b          : in  STD_LOGIC_VECTOR(7 downto 0);
            alu_op     : in  STD_LOGIC_VECTOR(3 downto 0);
            result     : out STD_LOGIC_VECTOR(7 downto 0);
            carry_out  : out STD_LOGIC;
            zero       : out STD_LOGIC
        );
    end component;

    component Memory is
        Port (
            clk         : in  STD_LOGIC;
            address     : in  STD_LOGIC_VECTOR(7 downto 0);
            data_in     : in  STD_LOGIC_VECTOR(7 downto 0);
            data_out    : out STD_LOGIC_VECTOR(7 downto 0);
            mem_write   : in  STD_LOGIC;
            mem_read    : in  STD_LOGIC
        );
    end component;

    component ControlUnit is
        Port (
            clk              : in  STD_LOGIC;
            reset            : in  STD_LOGIC;
            opcode           : in  STD_LOGIC_VECTOR(3 downto 0);
            carry_flag       : in  STD_LOGIC;
            zero_flag        : in  STD_LOGIC;
            pc_current       : in  STD_LOGIC_VECTOR(7 downto 0);
            mem_address      : out STD_LOGIC_VECTOR(7 downto 0);
            reg_write_enable : out STD_LOGIC;
            mem_read_enable  : out STD_LOGIC;
            mem_write_enable : out STD_LOGIC;
            alu_op           : out STD_LOGIC_VECTOR(3 downto 0);
            pc_update        : out STD_LOGIC_VECTOR(7 downto 0)
        );
    end component;

    -- Internal signals
    signal pc             : STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
    signal ir             : STD_LOGIC_VECTOR(7 downto 0);
    signal reg_data       : STD_LOGIC_VECTOR(7 downto 0);
    signal reg_out        : STD_LOGIC_VECTOR(7 downto 0);
    signal alu_result     : STD_LOGIC_VECTOR(7 downto 0);
    signal carry_out      : STD_LOGIC;
    signal zero_flag      : STD_LOGIC;
    signal reg_write_enable: STD_LOGIC;
    signal mem_read_enable: STD_LOGIC;
    signal mem_write_enable: STD_LOGIC;
    signal alu_op         : STD_LOGIC_VECTOR(3 downto 0);

begin

    -- Instantiate the Control Unit
    CU: ControlUnit
        port map (
            clk             => clk,
            reset           => reset,
            opcode          => ir(7 downto 4),
            carry_flag      => carry_out,
            zero_flag       => zero_flag,
            pc_current      => pc,
            mem_address     => mem_address,
            reg_write_enable => reg_write_enable,
            mem_read_enable => mem_read_enable,
            mem_write_enable => mem_write_enable,
            alu_op          => alu_op,
            pc_update       => pc
        );

    -- Instantiate the Register File
    RegFile: RegisterFile
        port map (
            clk        => clk,
            reset      => reset,
            reg_write  => reg_write_enable,
            reg_data   => alu_result,
            reg_addr   => ir(3 downto 2),  -- Register address from instruction
            reg_out    => reg_out
        );

    -- Instantiate the ALU
    ALU: ALU
        port map (
            a         => reg_out,
            b         => mem_data_in,  -- Assuming we use memory data as input for ALU operations
            alu_op    => alu_op,
            result    => alu_result,
            carry_out => carry_out,
            zero      => zero_flag
        );

    -- Instantiate the Memory
    RAM: Memory
        port map (
            clk        => clk,
            address    => mem_address,
            data_in    => reg_out,
            data_out   => mem_data_in,
            mem_write  => mem_write_enable,
            mem_read   => mem_read_enable
        );

    -- Instruction Register
    process(clk, reset)
    begin
        if reset = '1' then
            ir <= (others => '0');
        elsif rising_edge(clk) then
            if mem_read_enable = '1' then
                ir <= mem_data_in;
            end if;
        end if;
    end process;

    -- Program Counter Update
    process(clk, reset)
    begin
        if reset = '1' then
            pc <= (others => '0');
        elsif rising_edge(clk) then
            if mem_read_enable = '1' or mem_write_enable = '1' then
                pc <= CU.pc_update;
            end if;
        end if;
    end process;

end Behavioral;
