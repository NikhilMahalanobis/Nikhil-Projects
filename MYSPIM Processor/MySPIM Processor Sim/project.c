// This file WAS written by Nikhil Mahalanobis
// CDA 3103, Fall 2019

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  switch (ALUControl)
  {
    // Addition: Z = A + B
    case 0: *ALUresult = A + B;
            break;

    // Subtraction: Z = A - B
    case 1: *ALUresult = A - B;
            break;

    // Set less than signed: If A < B, Z = 1; otherwise, Z = 0
    case 2: if ((int)A < (int)B)
              *ALUresult = 1;
            else
              *ALUresult = 0;
              break;
    // Set less than unsigned: If A < B, Z = 1; otherwise, Z = 0
    case 3: if (A < B)
              *ALUresult = 1;
            else
              *ALUresult = 0;
              break;
    // AND: Z = A AND B
    case 4: *ALUresult = A & B;
            break;
    // OR: Z = A OR B
    case 5: *ALUresult = A | B;
            break;
    // Shift: Shift left B by 16 bits
    case 6: *ALUresult = B << 16;
            break;
    // NOT: Z = NOT A
    case 7: *ALUresult = ~A;
  }

  if (*ALUresult == 0)
    *Zero = 1;
  else
    *Zero = 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  // 1. Fetch the instruction addressed by PC from Mem and write it to instruction.
  // 2. Return 1 if a halt condition occurs; otherwise, return 0.
  // According to FAQ, ALU will halt if Mem is not byte addresses or goes outside 64k limit, so need to figure out word alignment
  // mod by 4


  if (PC > 65336 || ((PC % 4) != 0))
    return 1;

  else
  {
    *instruction = Mem[PC >> 2];
    return 0;
  }
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
  // 31-26, we want upper 6 bits, so shift by 26, then bitwise AND with lower 6 bits asserted
  *op = (instruction >> 26) & 0b00000000000000000000000000111111;

  // 25-21, shift by 21 then bitwise and for first 5 bits
  *r1 = (instruction >> 21) & 0b11111;

  // 20-16, shift by 16, then bitwise and for first 5 bits
  *r2 = (instruction >> 16) & 0b11111;

  //15-11, shift by 11, then bitwise and for first 5 bits
  *r3 = (instruction >> 11) & 0b11111;

  //5-0, bitwise and for first 6 bits?
  *funct = instruction & 0b00000000000000000000000000111111;

  //15-0, bitwise and for first 16 bits?
  *offset = instruction & 0b00000000000000001111111111111111;

  //25-0, bitwise and for first 26 bits?
  *jsec = 0b00000011111111111111111111111111;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
  // Easier to organize by instrution type
  // First do R type, then I type, then J Type
  switch (op)
  {
    // First is R-Type instruction, we assert RegDst and RegWrite, set ALUOp to 111 (7)
    case 0: controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;

    // Now I-Types; beq, addi, lw, sw, lui, slti, sltiu

    // beq, we assert Branch and ALUSrcs, we don't care about RegDst and MemtoReg, and set ALUOp to 0 to calculate address
    case 4: controls->RegDst = 2;
             controls->Jump = 0;
             controls->Branch = 1;
             controls->MemRead = 0;
             controls->MemtoReg = 2;
             controls->ALUOp = 0;
             controls->MemWrite = 0;
             controls->ALUSrc = 1;
             controls->RegWrite = 0;
             break;

    // addi, assert RegWrite and ALUSrc, deasserte everything else
    case 8: controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

    // slti, assert RegWrite and ALUSrc, and set ALUOp to 2 for slt comparison
    case 10: controls->RegDst = 0;
             controls->Jump = 0;
             controls->Branch = 0;
             controls->MemRead = 0;
             controls->MemtoReg = 0;
             controls->ALUOp = 2;
             controls->MemWrite = 0;
             controls->ALUSrc = 1;
             controls->RegWrite = 1;
             break;

    //sltiu, assert RegWrite and ALUSrcs, and set ALUOp to 3 for sltu comparison
    case 11: controls->RegDst = 0;
             controls->Jump = 0;
             controls->Branch = 0;
             controls->MemRead = 0;
             controls->MemtoReg = 0;
             controls->ALUOp = 3;
             controls->MemWrite = 0;
             controls->ALUSrc = 1;
             controls->RegWrite = 1;
             break;

      //lui, Assert RegWrite and ALUSrc, and set ALUOp to 6 for the 16 bit shift
      case 15: controls->RegDst = 0;
               controls->Jump = 0;
               controls->Branch = 0;
               controls->MemRead = 0;
               controls->MemtoReg = 0;
               controls->ALUOp = 6;
               controls->MemWrite = 0;
               controls->ALUSrc = 1;
               controls->RegWrite = 1;
               break;

      // load word, Assert RegWrite, ALUSrc, and MemRead, and MemtoReg, and set ALUOp to 0 to calculate address
      case 35: controls->RegDst = 0;
               controls->Jump = 0;
               controls->Branch = 0;
               controls->MemRead = 1;
               controls->MemtoReg = 1;
               controls->ALUOp = 0;
               controls->MemWrite = 0;
               controls->ALUSrc = 1;
               controls->RegWrite = 1;
               break;

      // store word, assert MemWrite and ALUSrcs, and we don't care about RegDst and MemtoReg, set ALUOp to 0 to calculate address
      case 43: controls->RegDst = 2;
               controls->Jump = 0;
               controls->Branch = 0;
               controls->MemRead = 0;
               controls->MemtoReg = 2;
               controls->ALUOp = 0;
               controls->MemWrite = 1;
               controls->ALUSrc = 1;
               controls->RegWrite = 0;
               break;

      // Last is J-Types, Jump
      // jump, we only asser jump, and we don't care about RegDst, ALUSrc, MemtoReg, or ALUOp, but we set ALUOp to 0 for No op??
      case 2:  controls->RegDst = 2;
               controls->Jump = 1;
               controls->Branch = 0;
               controls->MemRead = 0;
               controls->MemtoReg = 2;
               controls->ALUOp = 0;
               controls->MemWrite = 0;
               controls->ALUSrc = 2;
               controls->RegWrite = 0;
               break;
      default: return 1;

  }

  return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
  // Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively.
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
  // Assign the sign-extended value of offset to extended_value.
  // 16 bit sign extension, so take offset and bitwise AND with the 16th bit to see the value;
  // if its 1, the number will be negative and we extend it using uper 16 bits, otherwise don't do anything

  if ((offset & 0b1000000000000000) == 1)
    *extended_value = (offset | 0b11111111111111110000000000000000);
  else
    *extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  // Apply ALU operations on data1, and data2 or extended_value (determined byALUSrc).
  // The operation performed is based on ALUOp and funct.
  // Apply the function ALU(…).
  // Output the result to ALUresult.
  //Return 1 if a halt condition occurs; otherwise, return 0

  //Since ALUSrc controls our action, we have 2 scenarios:
  // If ALUSrc == 1, we know its an I-Type, and we can pass parameters through ALU function
  // If ALUSrc == 0, we know its an R-type, so we need to assign the ALUop based on funct decode

  if (ALUSrc == 1)
  {
    data2 = extended_value;
    ALU(data1, data2, ALUOp, ALUresult, Zero);
  }

  else if (ALUSrc == 0)
  {
    switch (funct)
    {
      // shift
      case 4: ALUOp = 6;
              break;

      // add
	    case 32: ALUOp = 0;
    	    		 break;
	    // sub
	    case 34: ALUOp = 1;
      	    	 break;

      // AND
      case 36: ALUOp = 4;
               break;
      // OR
      case 37: ALUOp = 5;
               break;

      // NOT
      case 39: ALUOp = 7;
               break;

      // slt signed
	    case 42: ALUOp = 2;
      	    	 break;
	    // slt unsigned
	    case 43: ALUOp = 3;
      	    	 break;

	    // default is halt
	    default: return 1;
    }

    ALU(data1, data2, ALUOp, ALUresult, Zero);
  }
  return 0;
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  // Base on the value of MemWrite or MemRead to determine memory write operation or memory read operation.
  // Read the content of the memory location addressed by ALUresult to memdata.
  //  Write the value of data2 to the memory location addressed by ALUresult.
  //Return 1 if a halt condition occurs; otherwise, return 0.

  // Check if MemWrite == 1 OR MemRead == 1, and if one of them is, check that our location is correctly byte addressed (%4), if not then halt
  // If we're good, then if we Write, then multiply ALUResult by 4 (shift by 2) and assign from data2
  // If Read, then set memdata to ALUResult * 4 (shift by 2)

  if (MemRead == 1 || MemWrite == 1)
  {
    if ((ALUresult % 4) != 0)
      return 1;
  }

  if (MemWrite == 1)
  {
    Mem[ALUresult >> 2] = data2;
  }
  if (MemRead == 1)
  {
    *memdata = Mem[ALUresult >> 2];
  }

  return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  // Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3
  // if RegWrite == 1, we know we're writing, but we need to check MemtoReg and RegDst to see where we're writing to and from
  // if MemReg == 1 and RegDst == 1, write to r3 from memory
  // if MemReg == 1 and RegDst == 0, write to r2 from memory
  // if MemReg == 0 and RegDst == 1, write to r3 from ALUResult
  // if MemReg == 0 and RegDst == 0, write to r2 from ALUResult

  if (RegWrite == 1)
  {
    if (MemtoReg == 1 && RegDst == 1)
      Reg[r3] = memdata;
    else if (MemtoReg == 1 && RegDst == 0)
      Reg[r2] = memdata;
    else if (MemtoReg == 0 && RegDst == 1)
      Reg[r3] = ALUresult;
    else if (MemtoReg == 0 && RegDst == 0)
      Reg[r2] = ALUresult;
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
  // ALways PC + 4, never not PC + 4

  *PC += 4;

  // cases where we do more than PC + 4; Jumps and Branch
  // If we jump, we must we need to take jsec and and shift by 2, then bitwise OR with upper 4 bits of PC
  // If we branch, check to see if Zero is asserted. If so, then PC will be the sign_extension shifted twice
  if (Jump == 1)
  {
    *PC = (jsec << 2) | (*PC | 0b11110000000000000000000000000000);
  }

  else if (Branch == 1 && Zero == 1)
  {
    *PC += (extended_value << 2);
  }

}
