컴시구 중간고사
=

## 기초지식

**Algorithm: 실행 작업의 개수를 결정**   

**Programming language, compiler, architecture:**   
작업 당 cpu 명령어의 개수를 결정

**Processor and memory system**  
cpu명령어의 실행 속도를 결정

**I/O system (including OS)**   
I/O 작업의 실행 속도를 결정

**암달의 법칙:**  
프로그램은 병렬처리가 가능한 부분과 불가능한 순차적인 부분으로 구성되므로 프로세서를 아무리 병렬화 시켜도 더 이상 성능이 향상되지 않는 한계가 존재 한다는 법칙
$${1 \over (1 - P) + {P \over S}}$$
어떤 작업의 40%에 해당하는 부분의 속도를 2배로 늘릴 수 있다면, P는 0.4이고 S는 2이고 최대 성능 향상은
$${1 \over (1 - 0.4) + {0.4 \over 2}} = 1.25$$
가 된다.

**무어의 법칙:**  
반도체 집적회로의 성능이 24개월마다 2배로 증가한다는 법칙

**Seven Great Ideas**:   
- Use abstraction to simplify design
- Make the common case fast
- Performance via parallelism
- Performance via pipelining
- Performance via prediction
- Hierarchy of memories
- Dependability via redundancy

---

## CPU Clocking

Performance = 1/Execution Time

**Clock Cycle Time(period):**   
duration of a clock cycle   
싸이클 한 번에 걸리는 시간

**Clock Rate (frequency):**   
cycles per second   
1초에 돌아가는 싸이클의 수

**Clock Cycles:**  
Clock Cycles = Instruction Count * Cycles per Instruction (CPI)   
전체 Cycle의 개수 = 명령 개수 * 명령당 싸이클 개수

**CPU Time:**   
CPUTime = Clock Cycles * Clock Cycle Time   
= Clock Cycles / Clock Rate   
총 실행 시간 = 싸이클의 개수 * 싸이클당 시간

**예제**:   
- Computer A: Cycle Time = 250ps, CPI = 2.0
- Computer B: Cycle Time = 500ps, CPI = 1.2

CPUTime = Instruction Count * CPI * Cycle Time   
A = IC * 2.0 * 250 = IC * 500   
B = IC * 1.2 * 500 = IC * 600   
CPUTime_B / CPUTime_A = 1.2   
A가 1.2배 빠르다

---

## MIPS

빅엔디안 시스템. 32bit 레지스터 32개가 있음

- $zero: constant zero
- $a0 – $a3: arguments (reg's 4 – 7)
- $v0, $v1: result values (reg’s 2 and 3)
- $t0 – $t9: temporaries   
	- Can be overwritten by callee
- $s0 – $s7: saved
	- Must be saved/restored by callee
- $gp: global pointer for static data (reg 28)
- $sp: stack pointer (reg 29)
- $fp: frame pointer (reg 30)
- $ra: return address (reg 31)

이외에도 목적별로 레지스터가 많음.
전체 번호는 $0 ~ $31임

**R-format instruction fields:**   
일반적인 인스트럭션의 구성

|op|rs|rt|rd|shamt|funct|
|--|--|--|--|-----|-----|
|6bits|5bits|5bits|5bits|5bits|6bits|

- op: operation code (opcode)
- rs: first source register number
- rt: second source register number
- rd: destination register number
- shamt: shift amount (00000 for now)
- funct: function code (extends opcode)

opcode는 대분류 (산술, 논리), functs는 소분류 (나눗셈, 덧셈)

shamt는 shift연산 할 때만 쓰고, 다른 명령어에선 항상 0임. shift연산의 최댓값이 32라 I-format으로 안빼고 놔둔 것 같음

**I-format instruction fields:**   
즉시값 산술과 load/store 인스트럭션 구성

|op|rs|rt|imm or addr|
|--|--|--|-----------|
|6bits|5bits|5bits|16bits|

- rt: src, dest로 사용
- imm: -2^15 ~ 2^15 - 1
- addr: offset added to base address in rs

| 명령    | arg1 | arg2      | arg3 | 설명                                               |
|-------|------|-----------|------|--------------------------------------------------|
| add   | reg1 | reg2      | reg3 | reg1 = reg2 + reg3                               |
| sub   | reg1 | reg2      | reg3 | reg1 = reg2 - reg3                               |
| lw    | reg1 | imm(reg2) | X    | reg1 = *(reg2 + imm)                             |
| lh    | reg1 | imm(reg2) | X    | load half word (sign extend)                     |
| lhu   | reg1 | imm(reg2) | X    | load half word (zero extend)                     |
| lb    | reg1 | imm(reg2) | X    | load byte (sign extend)                          |
| lbu   | reg1 | imm(reg2) | X    | load byte (zero extend)                          |
| sw    | reg1 | imm(reg2) | X    | *(reg2 + imm) = reg1                             |
| sh    | reg1 | imm(reg2) | X    | save half word                                   |
| sb    | reg1 | imm(reg2) | X    | save byte                                        |
| addi  | reg1 | reg2      | imm  | reg1 = reg2 + imm                                |
| sll   | reg1 | reg2      | imm  | reg1 = reg2 << imm                               |
| srl   | reg1 | reg2      | imm  | reg1 = reg2 >> imm                               |
| and   | reg1 | reg2      | reg3 | reg1 = reg2 & reg3                               |
| andi  | reg1 | reg2      | imm  | reg1 = reg2 & imm                                |
| or    | reg1 | reg2      | reg3 | reg1 = reg2 &#124; reg3                          |
| ori   | reg1 | reg2      | imm  | reg1 = reg2 &#124; imm                           |
| nor   | reg1 | reg2      | reg3 | reg1 = ~(reg2 &#124; reg3)                       |
| beq   | reg1 | reg2      | L1   | jump L1 if reg1 == reg2                          |
| bne   | reg1 | reg2      | L1   | jump L1 if reg1 != reg2                          |
| j     | L1   | X         | X    | jump L1                                          |
| slt   | reg1 | reg2      | reg3 | reg1 = reg2 < reg3                               |
| slti  | reg1 | reg2      | imm  | reg1 = reg2 < imm                                |
| sltu  | reg1 | reg2      | reg3 | reg1 = reg2 < reg3 (unsigned)                    |
| sltui | reg1 | reg2      | imm  | reg1 = reg2 < imm  (unsigned)                    |
| jal   | L1   | X         | X    | $ra = {following inst addr}. jump L1 (proc call) |
| jr    | $ra  | X         | X    | jump $ra (proc return)                           |
| lui   | reg1 | imm       | X    | load imm to upper 16bits of reg1                 |
| ori   | reg1 | reg2      | imm  | reg1 = reg2 &#124; imm                           |
| ll    | reg1 | imm(reg2) | X    | reg1 = *(reg2 + imm)                             |
| sc    | reg1 | imm(reg2) | X    | *(reg2 + imm) = reg1                             |

ll (load linked) : 32비트 로드를 시작하면 CPU에 접근 가능 상태 비트가 설정된다.   
sc (Store Conditional) : ll이 로드한 주소가 sc에 의해 write되기 전에 다른 프로세서가 write했다면,
CPU 상태비트가 접근 불가능으로 바뀌고 sc는 write에 실패하게 됨. (성공시 reg1=1, 실패시 reg1=0)   
보통 sc가 write에 실패하면 ll로 load한 데이터를 폐기하고 다시 ll을 수행하게 swap을 구현함
