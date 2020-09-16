// Aakef Waris
// CS-350
// Assignment: Final Project - Disassembler
// Due Date: May 1st 2019
//////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// structs for different types
struct rFormat { char * rs; char * rt; char * rd; unsigned int shamt; char * funct; };
struct iFormat { char * rs; char * rt; short  immediate; };
struct jFormat { unsigned int address; };


// instruction format
struct instruction {
  char * opcode;
  char type;
  union {

    struct rFormat r;
    struct iFormat i;
    struct jFormat j;

  };
};



// returns shift amt
unsigned int getShamt(unsigned int bit32){
  unsigned int sh_mask = 0x000007C0;
  unsigned int shamt = (sh_mask & bit32) >> 6;
  return shamt;
}

// returns funct code for r types
char * getFunct(unsigned int bit32){

  unsigned int f_mask = 0x0000003F;
  unsigned int funct = (f_mask & bit32) ;
  
  
  if(funct == 32){
    return "add  ";
  }
  else if(funct == 36){
    return "and  ";
  }
  else if(funct == 26){
    return "div  ";
  }
  else if(funct == 8){
    return "jr   ";
  }
  else if(funct == 24){
    return "mult ";
  }
  else if(funct == 39){
    return "nor  ";
  }
  else if(funct == 37){
    return "or   ";
  }
  else if(funct == 42){
    return "slt  ";
  }
  else if(funct == 34){
    return "sub  ";
  }
  else if(funct == 40){
    return "xor  ";
  }
  else if(funct == 0 && getShamt(bit32) == 0){
    return "nop  ";
  }
  else if(funct == 0 && getShamt(bit32) != 0){
    return "sll  ";
  }
  return "     ";
}


// gets registers given an integer for printing
char * getReg(unsigned int code){
	if(code == 0){
		return ("$0 ");
	}
	else if(code == 1){
		return("$at");
	}
	else if(code == 2){
		return("$v0");
	}
	else if(code == 3){
		return("$v1");
	}
	else if(code == 4){
		return("$a0");
	}
	else if(code == 5){
		return("$a1");
	}
	else if(code == 6){
		return("$a2");
	}
	else if(code == 7){
		return("$a3");
	}
	else if(code == 8){
		return("$t0");
	}
	else if(code == 9){
		return("$t1");
	}
	else if(code == 10){
		return("$t2");
	}
	else if(code == 11){
		return("$t3");
	}
	else if(code == 12){
		return("$t4");
	}
	else if(code == 13){
		return("$t5");
	}
	else if(code == 14){
		return("$t6");
	}
	else if(code == 15){
		return("$t7");
	}
	else if(code == 16){
		return("$s0");
	}
	else if(code == 17){
		return("$s1");
	}
	else if(code == 18){
		return("$s2");
	}
	else if(code == 19){
		return("$s3");
	}
	else if(code == 20){
		return("$s4");
	}
	else if(code == 21){
		return("$s5");
	}
	else if(code == 22){
		return("$s6");
	}
	else if(code == 23){
		return("$s7");
	}
	else if(code == 24){
		return("$t8");
	}
	else if(code == 25){
		return("$t9");
	}
	else if(code == 26){
		return("$k0");
	}
	else if(code == 27){
		return("$k1");
	}
	else if(code == 28){
		return("$gp");
	}
	else if(code == 29){
		return("$sp");
	}
	else if(code == 30){
		return("$fp");
	}
	else if(code == 31){
		return("$ra");
	}
}

// returns an register of integer value for the rd field 
char * getRD(unsigned int bit32){
  unsigned int rd_mask = 0x0000F800;
  unsigned int rd = (rd_mask & bit32) >> 11;
  
  return getReg(rd);
}

// returns an register of integer value for the rt field
char * getRT(unsigned int bit32){
  unsigned int rt_mask = 0x001F0000;
  unsigned int rt =  (rt_mask & bit32) >> 16;
  
  return getReg(rt);
} 

// returns an register of integer value for the rs field
char * getRS(unsigned int bit32){
  unsigned int rs_mask = 0x03E00000;
  unsigned int rs =  (rs_mask & bit32) >> 21 ;
  
  return getReg(rs);
}

// returns immeditate field for i type instructions
short getImmediate(unsigned int bit32){
  short im_mask = 0xFFFF;
  short immediate = (im_mask & bit32);
  return immediate;
}

// returns address filed for j type instructions
unsigned int getAddress(unsigned int bit32){
  unsigned int a_mask = 0x03FFFFFF;
  unsigned int address = (bit32 & a_mask);
  return address;
}


// gets the opcodes for all instructions, calls get funct for r type instructins if opcode is 0
char * getOpcode(unsigned int bit32){
  unsigned int o_mask = 0xFC000000;
  unsigned int op = (o_mask & bit32) >> 26;
  if (op == 0){
    return getFunct(bit32);
  }

  else if (op == 2){
    return "j    ";
  }
  else if (op == 3){
    return "jal  ";
  }
  else if (op == 8){
    return "addi ";
  }
  else if (op == 4){
    return "beq  ";
  }
  else if (op == 5){
    return "bne  ";
  }
  else if (op == 15){
    return "lui  ";
  }
  else if (op == 35){
    return "lw   ";
  }
  else if (op == 13){
    return "ori  ";
  }
  else if (op == 10){
    return "slti ";
  }
  else if (op == 43){
    return "sw   ";
  }
  else if (op == 12){
    return "andi ";
  }
  else if (op == 14){
    return "xori ";
  }

  return "     ";
}

int main (void){



  printf("Loading data.txt...\n");
  // FILE IO

  // Insert test file in place of current file name
  char *txt = "FP_packet/data.txt";
  FILE *file = fopen(txt, "r");

  // exit if file cannot be opened or file not found
  if (!file) {
  printf("; open failed; program ends\n");
  exit(EXIT_FAILURE);
  }
  
  unsigned int hexValue;
  int length = 0;

  // get instrucitons to get allocate proper amount of space in array of structs
  while(!feof(file)){
    length++;
    fscanf(file, "%x", &hexValue);
  }
  fclose(file);

  struct instruction data [length]; // hold mips instructions
  unsigned int hexCodes [length]; // holds hex codes

  // Re-read file after counting instuctions and getting length
  char *txt2 = "FP_packet/data.txt";
  FILE *file1 = fopen(txt2, "r");

  // exit if file cannot be opened or file not found
  if (!file1) {
  printf("; open failed; program ends\n");
  exit(EXIT_FAILURE);
  }

  // loop goes through text file once again to place instructions in struct arr and hex in hex arr
  int counter = 0;
  int opcode;

  while(!feof(file1)){
    fscanf(file1, "%08x", &hexValue);  

    // Inserting hex code and opcodes into respective arrays
    hexCodes[counter] = hexValue; 
    data[counter].opcode = getOpcode(hexValue);

    // get opcode
    unsigned int op = (0xFC000000 & hexValue) >> 26;

    // handles R type instructions
    if (op == 0){
      data[counter].type = 'r';
      data[counter].r.rs = getRS(hexValue);
      data[counter].r.rt = getRT(hexValue);
      data[counter].r.rd =  getRD(hexValue);
      data[counter].r.shamt = getShamt(hexValue);
    }

    // handles J type instructions 
    else if (op == 2 || op == 3){
      data[counter].type = 'j';
      data[counter].j.address = getAddress(hexValue);
    }

    // handles I type instructions
    else{
      data[counter].type = 'i';
      data[counter].i.rs =  getRS(hexValue);
      data[counter].i.rt =  getRT(hexValue);
      data[counter].i.immediate = getAddress(hexValue);
    }
    counter++;
  }

  fclose(file1);

  // takes PC counter input from user
  unsigned int PC_COUNTER = 0;
  printf("Read in %d lines of data\n", length-1);
  printf("Enter a starting hex value for the program counter: \n");
  printf("Initial Program Counter: ");
  scanf(" %08x", &PC_COUNTER);


  // loop through arrays to print instructions
  printf("\nPC Address   Instruction");
  int idx;
  for (idx=0; idx < length - 1; idx++){
    printf("\n0x%08X   ", PC_COUNTER);
    printf("%.8s", data[idx].opcode);

    // handles r type instructions
    if (data[idx].type == 'r'){

      // handles weird r types that dont use rs
      if (data[idx].opcode == "sll  " || data[idx].opcode == "nop  "){
        printf("%s %s %-8x (rs/f: 0x%02x 0x%02x = dec %2d %2d)", data[idx].r.rd, data[idx].r.rt, data[idx].r.shamt, (0x03E00000 & hexCodes[idx]) >> 21, (0x0000003F&hexCodes[idx]), 
        (0x03E00000 & hexCodes[idx]) >> 21, (0x0000003F&hexCodes[idx]));
        printf("\n");
      }
      
      // handles general case r types that us rs
      else{
        printf("%s %s %-8s (s/f: 0x%02x 0x%02x = dec %2d %2d)", data[idx].r.rd, data[idx].r.rs, data[idx].r.rt,  (0x000007C0 & hexCodes[idx]) >> 6, (0x0000003F&hexCodes[idx]), 
        (0x000007C0 & hexCodes[idx]) >> 6, (0x0000003F&hexCodes[idx]));
        printf("\n");
      }

    }

    // handles i type instructions
    else if(data[idx].type == 'i'){

      // handles i type that branches
      if (data[idx].opcode == "beq  " || data[idx].opcode == "bne  "){
         unsigned int target = (PC_COUNTER + 4) + (4 * (data[idx].i.immediate) );
         printf("%s %s 0x%08x  : dec %-8d target = %x", data[idx].i.rs, data[idx].i.rt, (data[idx].i.immediate), (data[idx].i.immediate) , target );
         printf("\n");
      }

      // handles general i types that dont branch
      else{
      printf("%s %s 0x%08x  : dec %d", data[idx].i.rs, data[idx].i.rt, (data[idx].i.immediate) , (data[idx].i.immediate)  );
      printf("\n");
      }

    }

    // handles j type instructions 
    else{
      int target;
      int next;
      target = data[idx].j.address << 2;
      next = (PC_COUNTER + 4) >> 28;

      printf("%08x\n", data[idx].j.address);
    }
    
    // increment PC counter for each instruction
    PC_COUNTER += 4;
  }

  printf("\nProgram Ending...");
  exit(0);

  return 0;
}
