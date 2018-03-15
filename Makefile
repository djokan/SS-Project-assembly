FLAGS= -std=gnu++0x
INC_DIR=inc/ inc/Instructions/ inc/Operands
SRC_DIR=src/
INSTRUCTIONS=ArLog.cpp Stack.cpp LdSt.cpp Segment.cpp Define.cpp Branch.cpp
INSTRUCTIONS_DIR= Instructions/
ABS_INSTRUCTIONS=$(addprefix $(INSTRUCTIONS_DIR),$(INSTRUCTIONS))
OPERANDS=Imm.cpp RegDir.cpp RegInd.cpp RegIndPom.cpp MemDir.cpp PcRel.cpp
OPERANDS_DIR= Operands/
ABS_OPERANDS=$(addprefix $(OPERANDS_DIR),$(OPERANDS))
SRC=Utilities.cpp Parser.cpp main.cpp Instruction.cpp ProgramContext.cpp Operand.cpp Expr.cpp $(ABS_INSTRUCTIONS) $(ABS_OPERANDS)
ABS_SRC=$(addprefix $(SRC_DIR),$(SRC))
TARGET=../test/asm
INCLUDE=$(addprefix -I,$(INC_DIR))
project: $(ABS_SRC) $(ABS_INC)
	g++ $(FLAGS) -o $(TARGET) $(INCLUDE) $(ABS_SRC)
clean:
	rm -f $(TARGET)
