#include <stdio.h>
#include <stdlib.h>
#define BYTE unsigned char

typedef struct funcLL {
	struct funcLL *next;
	struct funcLL *prev;
    int byteCount;
	int instrCount;
    int funcID;
	int *opIndexes;
	BYTE *data;
} funcL_t;

BYTE readBits ( int toRead, int *bitsRead, BYTE *stream, int *byteCount, int *ptrIndex){
	int bitsLeft = (sizeof(BYTE)*8 - *bitsRead);
	if (bitsLeft < toRead){
		if (*ptrIndex > 0){
			BYTE tmp = (stream[*ptrIndex - 1]);
			tmp = tmp<<(sizeof(BYTE)*8-(toRead - bitsLeft));
			tmp = tmp>>(sizeof(BYTE)*8-(toRead));

			tmp = tmp|stream[*ptrIndex];
			*bitsRead = *bitsRead + toRead;
			*byteCount = *byteCount+1;
			*bitsRead = toRead - bitsLeft;
			*ptrIndex = *ptrIndex-1;
			stream[*ptrIndex] = stream[*ptrIndex]>>(toRead - bitsLeft);
			return (BYTE)tmp;
		} else {
			return (BYTE)0;
		}
	}

	BYTE currCommand = stream[*ptrIndex]<<(((sizeof(BYTE)*8) - toRead));
	currCommand = currCommand>>(((sizeof(BYTE)*8) - toRead));


	*bitsRead = *bitsRead + toRead;
	stream[*ptrIndex] = stream[*ptrIndex]>>toRead;
	if (*bitsRead == (sizeof(BYTE)*8)) {
		*ptrIndex = *ptrIndex -1;
		*bitsRead = 0;
	}
	*byteCount = *byteCount+1;
	return (BYTE)currCommand;
}

typedef enum {
	MOV,
	CAL,
	POP,
	RET,
	ADD,
	AND,
	NOT,
	EQU
} Opcodes;

BYTE* addFunction (int *ptrIndex, int *bitsRead, int *byteCount, BYTE *ptr,int *functionCommands,int *funcID, int *opIndexes){
	int commandCount = 0;

	BYTE *function = malloc(sizeof(BYTE)*(*functionCommands*5 + 2));
	if(function == NULL){
		fprintf(stderr, "Unable to Allocate memory for BYTE *function\n");
		exit(1);
	}
	BYTE aType;

	while (commandCount < *functionCommands) {

		function[*byteCount-1] = readBits(3, bitsRead, ptr, byteCount, ptrIndex);
		BYTE nextOp = function[*byteCount -1];
		opIndexes[(*functionCommands-1) - commandCount] = *byteCount -1;

		commandCount = commandCount + 1;

		switch ((int)nextOp) {
			case MOV:
				aType = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
				switch ((int)aType) {
					case 1:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(2,bitsRead, ptr, byteCount, ptrIndex);
						switch ((int)function[*byteCount-1]) {
							case 0:
								function[*byteCount-1] =  readBits(8,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 1:
								function[*byteCount-1] =  readBits(3,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 2:
								function[*byteCount-1] =  readBits(7,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 3:
								function[*byteCount-1] =  readBits(7,bitsRead, ptr, byteCount, ptrIndex);
								break;
						}
						break;
					case 2:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(7,bitsRead, ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(2,bitsRead, ptr, byteCount, ptrIndex);
						switch ((int)function[*byteCount-1]) {
							case 0:
								function[*byteCount-1] =  readBits(8,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 1:
								function[*byteCount-1] =  readBits(3,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 2:
								function[*byteCount-1] =  readBits(7,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 3:
								function[*byteCount-1] =  readBits(7,bitsRead, ptr, byteCount, ptrIndex);
								break;
						}
						break;
					case 3:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(7,bitsRead, ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(2,bitsRead, ptr, byteCount, ptrIndex);
						switch ((int)function[*byteCount-1]) {
							case 0:
								function[*byteCount-1] =  readBits(8,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 1:
								function[*byteCount-1] =  readBits(3,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 2:
								function[*byteCount-1] =  readBits(7,bitsRead, ptr, byteCount, ptrIndex);
								break;
							case 3:
								function[*byteCount-1] =  readBits(7,bitsRead, ptr, byteCount, ptrIndex);
								break;
						}
						break;
				}
				break;
			case CAL:
				aType = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
				switch ((int)aType) {
					case 2:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(7,bitsRead,ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(2,bitsRead, ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(8,bitsRead, ptr, byteCount, ptrIndex);
						break;
					case 3:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(7,bitsRead,ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(2,bitsRead, ptr, byteCount, ptrIndex);
						function[*byteCount-1] = readBits(8,bitsRead, ptr, byteCount, ptrIndex);
						break;
				}
                break;
			case POP:
				aType = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
				switch ((int)aType) {
					case 1:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
						break;
					case 2:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(7,bitsRead,ptr, byteCount, ptrIndex);
						break;
					case 3:
						function[*byteCount-1] = aType;
						function[*byteCount-1] = readBits(7,bitsRead,ptr, byteCount, ptrIndex);
						break;
				}
                break;
			case RET:
				break;
			case ADD:
				function[*byteCount-1] = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
				function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
				function[*byteCount-1] = readBits(2,bitsRead, ptr, byteCount, ptrIndex);
				function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
				break;
			case AND:
				function[*byteCount-1] = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
				function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
				function[*byteCount-1] = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
				function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
				break;
			case NOT:
					function[*byteCount-1] = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
					function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
					break;
			case EQU:
					function[*byteCount-1] = readBits(2, bitsRead, ptr, byteCount, ptrIndex);
					function[*byteCount-1] = readBits(3,bitsRead, ptr, byteCount, ptrIndex);
					break;
		}
	}
	function[*byteCount-1] = readBits(4,bitsRead, ptr, byteCount, ptrIndex);
	function[*byteCount-1] = readBits(4,bitsRead, ptr, byteCount, ptrIndex);
    *funcID = function[*byteCount-1];

	BYTE *functionNew = realloc(function, sizeof(*function)*(*byteCount));
		if(functionNew == NULL){
			fprintf(stderr, "BAD REALLOC!!\n");
			free(function);
			exit(1);
		} else{
			function = functionNew;
		}
	return function;
}

funcL_t* getFunction(int label, funcL_t *listp){
	for ( ; listp != NULL; listp = listp->next ){
		if (listp->funcID == label){
			return listp;
		}
	}
	fprintf(stderr, "couldn't locate function label in list\n");
	return NULL;
}
//STACK METHODS

void push(BYTE *stack, BYTE byte, BYTE *sp){
    stack[(int)*sp] = byte;
    *sp = *sp+1;
	if(*sp > 127){
		free(stack);
		printf("Stack Overflow!\n");
		exit(1);
	}
}

BYTE pop(BYTE *stack, BYTE *sp){
    *sp = *sp-1;
    BYTE ret = stack[(int)*sp];
    stack[(int)*sp] = 0;
    return ret;
}

void newFrame(BYTE *stack, BYTE *fp, BYTE *sp, BYTE *pc){
	*fp = *sp;
	*sp = *fp + 2;
	pc = sp;
	*sp = *sp+1;
}

void MOVE(int fromType, int from, int toType, int to, BYTE *stack, BYTE *r, BYTE *fp, BYTE *sp){
	BYTE value = 0;
	BYTE ptr;
	switch ((int)fromType) {
		case 0:
			value = from;
			break;
		case 1:
			value = r[from];
			break;
		case 2:
			value = stack[*fp + from];
			break;
		case 3:

			ptr = stack[*fp + from];
			value = stack[(int)ptr];
			break;
	}
    switch ((int)toType) {
        case 1:
            r[to] = value;
            break;
        case 2:
            stack[*fp + to] = value;
			if (to > *sp){
				*sp = *sp + 1;
				if(*sp > 127){
					free(stack);
					printf("Stack Overflow!\n");
					exit(1);
				}
			}
            break;
        case 3:
            ptr = *(stack + (*fp + to));
            stack[(int)ptr] = value;
			*sp = *sp + 1;
			if(*sp > 127){
				free(stack);
				printf("Stack Overflow!\n");
				exit(1);
			}
            break;
    }
}

void CALL(funcL_t *toCall, BYTE *argsAddr, BYTE *stack, BYTE *fp, BYTE *sp, BYTE *pc, int *fpInd, int *framePointers){
	framePointers[*fpInd] = *fp;
	int argsN = toCall->data[toCall->byteCount - 2];
	newFrame(stack, fp, sp, pc);
	if(*sp > 127){
		free(stack);
		printf("Stack Overflow!\n");
		exit(1);
	}
	*fpInd = *fpInd + 1;
	for(int c = 0; c < argsN; c = c + 1){
		BYTE arg = argsAddr[c];
		push(stack, arg, sp);
		// printf("%d\n", *sp);
	}
}

void opPOP(BYTE *stack, BYTE *fp, int address, BYTE *returned){
	BYTE ret = stack[*fp + address];
	stack[*fp + address] = (BYTE)0;
	*returned = ret;
}

void RETURN(BYTE *stack, BYTE *fp, BYTE *sp, BYTE *pc, int *fpInd, int *framePointers, BYTE *returned){
	while(*sp != *fp || *sp > 4){
		if (*sp > 0){
			*sp = *sp - 1;
			stack[*sp] = 0;
		}

	}
	if (*fp > 0){
		stack[*sp-1] = *returned;
	} else {
		printf("%d\n", *returned);
		free(stack);
		exit(0);
	}
	*fpInd = *fpInd - 1;
	*fp = framePointers[*fpInd];
}

void opADD(BYTE *r,int a1, int a2){
	// printf("%d + %d = %d\n",r[a1], r[a2], r[a1] + r[a2]);
	r[a1] = r[a1] + r[a2];

	// printf("r[%d] =%d \n r[%d] = %d\n",a1, r[a1], a2, r[a2]);

}

void opAND(BYTE *r,int a1, int a2){
    r[a1] = r[a1]&r[a2];
}

void opNOT(BYTE *r,int addr){
    r[addr] = ~r[addr];
}

void opEQU(BYTE *r,int addr){
    if(r[addr] == 0){
        r[addr] = 1;
    } else {
        r[addr] = 0;
    }
}

int main(int n_args, char** vargs){
	//SCAFFOLD
    // For readability
    char* filename = vargs[1];
    // A pro-tip to prevent seg-faults for yourself might be to check
    // if this file actually exists, and if an argument is actually passed...

    // Create a 1 byte buffer. You might want more than one byte yourself
    BYTE buffer;

    // Open our file in 'read binary' mode,
    // I sure do hope that this file actually exists
    FILE *file_ptr = fopen(filename,"rb");
	if (file_ptr == NULL){
	    fprintf(stderr, "Not a valid file.\n");
	    return 1;
	}

    // Skip to the end of the file
    fseek(file_ptr, 0, SEEK_END);

    // Find the length of the file
    size_t file_length = ftell(file_ptr);

    // Rewind to the start of the file so we can actually read it
    rewind(file_ptr);

	BYTE *ptr = (BYTE *)malloc(sizeof(*ptr)*(file_length));
	if (ptr == NULL){
	    fprintf(stderr, "No Available Memory.\n");
	    return 1;
	}
    // For each byte in the file, first write the byte to our buffer
    // then print the integer representation of the byte

    for (int i = 0; i < file_length; i++)
    {
	    fread(&buffer, sizeof(BYTE), 1, file_ptr); // read 1 byte
		*(ptr+i) = buffer;

    }
	//SCAFFOLD END//
	//------------------------------//

    funcL_t *program = malloc(sizeof(*program)*1);

	int ptrIndex = file_length-1;
	int bitsRead = 0;
	int byteCount = 0;
    int funcID = 0;

	int functionCommands = (int)readBits(8, &bitsRead, ptr, &byteCount, &ptrIndex);
	byteCount = 0;

	program->instrCount = functionCommands;
	program->opIndexes = malloc(sizeof(int)*functionCommands);
    program->data = addFunction(&ptrIndex, &bitsRead, &byteCount, ptr, &functionCommands, &funcID, program->opIndexes);
    program->next=NULL;
    program->byteCount = byteCount;
    program->funcID = funcID;
	int functionCount = 1;

    functionCommands = (int)readBits(8, &bitsRead, ptr, &byteCount, &ptrIndex);
	funcL_t *next = malloc(sizeof(*next)*1);
	if(functionCommands>0){
		program->next=next;
		next->prev=program;
		next->opIndexes = malloc(sizeof(int)*functionCommands);
		next->instrCount = functionCommands;
		byteCount = 0;
	} else {
		free(next);
	}

	while (functionCommands > 0) {

		next->data = addFunction(&ptrIndex, &bitsRead, &byteCount, ptr, &functionCommands, &funcID, next->opIndexes);
		functionCount = functionCount + 1;
        next->byteCount = byteCount;
        next->funcID = funcID;
        next->next=NULL;
		functionCommands = (int)readBits(8, &bitsRead, ptr, &byteCount, &ptrIndex);
        if(functionCommands > 0 ){
            byteCount = 0;
            funcL_t *tmp = malloc(sizeof(*tmp)*1);
            next->next=tmp;
			tmp->prev=next;
            next = tmp;
			next->opIndexes = malloc(sizeof(int)*functionCommands);
			next->instrCount = functionCommands;
        }
	}

	free(ptr);

    // printf("%d\n\n", (int)program->funcID);
	// for (size_t c = 0; c < program->byteCount; c++) {
	// 	printf("%zu: %d\n", c, (int)program->data[c]);
	// }
	//
	// for (size_t c = 0; c < program->instrCount; c++) {
	// 	printf("%d\n", (int)program->opIndexes[c]);
	// }
    // printf("\n");
    // printf("%d\n", (int)program->next->funcID);
    // for (size_t x = 0; x < program->next->byteCount; x++) {
    //     printf("%d\n", (int)program->next->data[x]);
    //
    // }

    // THE STACK

	int fpInd = 0;

	int framePointers[functionCount];
	framePointers[fpInd] = 0;
	fpInd = fpInd+1;

    BYTE *r = malloc(sizeof(*r)*8);
    BYTE *stack = malloc(sizeof(*stack)*255);
    BYTE *fp = &stack[0];
	*fp = 0;
    BYTE *sp = &stack[1];
	*sp = (int)3;
    BYTE *pc = &stack[2];
    *pc = (int)1;
	BYTE *returned = malloc(sizeof(BYTE)*1);
	BYTE *returnTo = malloc(sizeof(BYTE)*1);
	*returnTo = 0;

    funcL_t *m = program;
	for (size_t c = 0; c < functionCount; c++) {
		if (m->funcID == 0){
			break;
		}
		m = m->next;
	}

	int count = 0;
	int caller = 0;
	int InstructCount = m->instrCount;
	int *counts = malloc(sizeof(*counts)*functionCount);
	funcL_t *lab;
	lab = program;
	for ( int x = 0; x < functionCount; x = x +1) {
		counts[lab->funcID] = 0;
		lab = lab->next;
	}

	for ( ;count < InstructCount ; count = count+1){
		int prevPC = (int)*pc;
		if(*sp > 127){
			free(stack);
			printf("Stack Overflow!\n");
			exit(1);
		}

		BYTE command =  m->data[m->opIndexes[count]];
		switch ((int)command) {
			case MOV:

				MOVE((int)m->data[m->opIndexes[count] + 3], (int)m->data[m->opIndexes[count] + 4], (int)m->data[m->opIndexes[count] + 1], (int)m->data[m->opIndexes[count] + 2], stack, r, fp,sp);
				int diff = (int)*pc - prevPC;
				count = count + diff;
				*pc = *pc + 1;
				break;
			case CAL:
				*pc = *pc +1;
				*returnTo = *sp;
				*sp = *sp + 1;
				if(*sp > 127){
					free(stack);
					printf("Stack Overflow!\n");
					exit(1);
				}

				int label = (int)m->data[m->opIndexes[count] + 4];
				caller = m->funcID;
				funcL_t *toCall;
				toCall = program;
				for ( ; toCall->funcID != label; ) {
			        toCall = toCall->next;
			    }
				CALL(toCall, &stack[*fp +(int)m->data[m->opIndexes[count] + 2]], stack, fp, sp, pc, &fpInd, framePointers);
				InstructCount = toCall->instrCount;
				counts[m->funcID] = count;
				count = -1;
				m = toCall;
				returnTo = fp;
				break;
			break;
			case POP:
				*pc = *pc +1;
				opPOP(stack, fp, (int)m->data[m->opIndexes[count] + 2], returned);
				break;
			case RET:
				RETURN(stack, fp, sp, pc, &fpInd, framePointers, returned);
				pc = fp + 2;
				label = (int)m->data[m->opIndexes[count] + 4];
				toCall = program;
				for ( ; toCall->funcID != caller; ) {
					m = toCall->next;
					toCall = toCall->next;
				}
				count = counts[caller];
				InstructCount = m->instrCount;
				break;
			case ADD:
				*pc = *pc +1;
				// printf("ADD\n");
				opADD(r, (int)m->data[m->opIndexes[count] + 4], (int)m->data[m->opIndexes[count] + 2]);
				break;
			case AND:
				*pc = *pc +1;
				opAND(r, (int)m->data[m->opIndexes[count] + 4], (int)m->data[m->opIndexes[count] + 2]);
				break;
			case NOT:
				*pc = *pc +1;
				opNOT(r, (int)m->data[m->opIndexes[count] + 2]);
				break;
			case EQU:
				*pc = *pc +1;
				opEQU(r, (int)m->data[m->opIndexes[count] + 2]);
				break;
		}
	}
	printf("Stack Overflow!\n");
	exit(1);

	funcL_t *p;
    for ( int c = 0; c < functionCount; c =c + 1) {
		p = program->next;
        free(program->data);
		free(program->opIndexes);
        free(program);
		program = p;
    }

	free(stack);
    free(r);
	free(counts);
	free(returned);
	free(returnTo);

    return 0;
}
