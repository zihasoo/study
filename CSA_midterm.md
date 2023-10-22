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

---

### **R-format instruction fields:**   
일반적인 인스트럭션 포멧

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

### **I-format instruction fields:**   
즉시값 산술과 load/store 를 위한 인스트럭션 포멧

|op|rs|rt|imm or addr|
|--|--|--|-----------|
|6bits|5bits|5bits|16bits|

- rt: src, dest로 사용
- imm: -2^15 ~ 2^15 - 1
- addr: offset added to base address in rs

주의할 점:   
I-format에서 점프를 할 때는 PC + 4 + (addr * 4) 위치로 점프한다고 생각해야 함.

### **J-format instruction fields:**   
점프를 위한 인스트럭션 포멧
|op|addr|
|--|----|
|6bits|26bits|

26비트면 꽤 큰편이지만, 32비트 전체를 표현하진 못함. 그래서 word주소를 사용 (addr * 4로 사용함)   
워드 주소를 사용하면 28비트까지는 표현 가능. 나머지 4비트는 PC의 상위 4비트를 그대로 가져옴. 이는 MIPS의 단점.

---

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

---

## MIPS Arithmetic

### **곱셈:**

32bit 곱셈의 결과는 64bit이고, MIPS는 32bit이기 때문에 결과를 레지스터 2개에 나눠서 저장함. 이때 높은 쪽이 HI, 낮은 쪽이 LO 레지스터에 저장됨. 아래는 이와 관련된 연산

mult rs, rt / multu rs, rt
- 곱셈의 결과(64비트)를 HI/LO 에 저장함

mfhi rd / mflo rd
- HI/LO에 있는 값을 rd로 옮김
- HI값을 통해 32비트 오버플로우가 발생했는지 확인 가능

mul rd, rs, rt
- 결과값의 하위 32bit만 rd에 저장

optimized multiplier:

1. product 레지스터(승수,피승수의 2배 크기)에 [0000 승수] 형태로 놓음
2. LSB가 1이면 상위 비트에 피승수를 더함.
3. shift right를 하고 2번부터 반복.
4. 승수가 모두 shift right 되면 최종 결과가 product 레지스터에 나옴

### **나눗셈:**

곱셈과 비슷하게 HI와 LO레지스터를 사용. HI에는 나머지, LO에는 몫이 들어감.

div rs, rt / divu rs, rt
- 오버플로우나 div by zero 체크 없음
- 곱셈과 마찬가지로 mfhi, mflo써서 결과 확인

optimized divider:

1. remainder 레지스터(제수,피제수의 2배 크기)에 [0000 피제수] 형태로 놓음
2. shift left를 함
3. 상위 비트에 (-제수)를 더함
4. 3번의 결과가 음수라면 (MSB가 1이라면) 3번을 시행하기 전으로 되돌림. 3번의 결과가 양수라면 LSB를 1로 세팅함. 2번부터 반복
5. 피제수가 모두 shift left되면 remainder 레지스터의 상위 절반 비트는 나머지, 하위 절반 비트는 몫이 됨.

### **부동소수점:**

**IEEE Floating-Point Format**

| S  | Exponent | Fractoin |
| -- | -------- | -------- |
|부호|지수부|가수부|

||S|Exponent|Fractoin|total|
|-|-|-------|--------|-----|
| single | 1bit | 8bit | 23 bit | 32bit |
| double | 1bit | 11bit | 52 bit | 64bit |

Normalize significand:   
소수를 표현할 때, $\pm X.XXX \times 10^n$ 으로 표현하는 방식을 정규화된 과학적 표기법(normalized scientific notation) 이라고 함. 이런 표기법을 이진수에 적용하면 앞부분이 [1.0, 2.0)의 값을 가지게 됨. 따라서 항상 1.0이 포함되므로, Fraction에는 1을 더해서 사용함.이를 hidden bit라고 함.

Exponent:   
지수부를 항상 unsigned로 저장하기위해 Bias라는 것을 사용함. 따라서 실제로 저장되는 값은 (지수부 + Bias)임. Bias는 단정밀도에서 127, 배정밀도에서 1023으로 사용함.

전체 표현식:   
$x = (-1) ^ S \times (1 + Fraction) \times 2^{(Exponent - Bias)}$

**표현 가능한 범위**

hidden bit를 사용하면 항상 1.xxx꼴로 나오기 때문에 0을 표현할 수가 없다. 따라서 아래와 같은 규칙을 사용한다.

지수부가 모두 0이고 가수부가 모두 0이면 이는 0을 의미한다.  
지수부가 모두 1이고 가수부가 모두 0이면 이는 무한대를 의미한다.

지수부가 모두 1이고 가수부가 0이 아니라면 이는 Not a number(Nan)이다.   
지수부가 모두 0이고 가수부 값을 모른다면 이는 denormal number이다.

(따라서 지수부 0..과 1..은 위의 의미로만 사용할 수 있다.)

single smallest
- 지수부: 00000001 (1 - 127 = -126)  
- 가수부: 000...00 (significand = 1.0)
- 최솟값: $\pm1.0 \times 2 ^{-126} \approx \pm1.2 \times 10^{-38} $   

single largest
- 지수부: 11111110 (254 - 127 = +127)   
- 가수부: 111...11 (significand $\approx$ 2.0)
- 최댓값: $\pm2.0 \times 2 ^{+127} \approx \pm3.4 \times 10^{+38} $   

double smallest
- 지수부: 00000000001 (1 - 1023 = –1022)   
- 가수부: 000...00 (significand = 1.0)
- 최솟값: $\pm1.0 \times 2 ^{-1022} \approx \pm2.2 \times 10^{-308} $   

double largest
- 지수부: 11111111110 (2046 - 1023 = +1023)   
- 가수부: 111...11 (significand $\approx$ 2.0)
- 최댓값: $\pm2.0 \times 2 ^{+1023} \approx \pm1.8 \times 10^{+308} $   

single의 경우 대략 10진수 6자리의 정밀도를 가지고, double은 약 16자리의 정밀도를 가진다.
