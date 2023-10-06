# 멀티스레드 정리

1. ref, future-promise, packaged_task, async의 속도 차이는 없다.
	- single_sum(): 5.506ms   
	multi_sum(): 1.81593ms   
	multi_sum_pf(): 1.82033ms   
	multi_sum_pkt(): 1.87113ms   
	multi_sum_async(): 1.8104ms
	- async는 스레드풀을 쓴다는 말이 있던데 위의 결과처럼 더 빠르게 나오진 않았다.

2. 지역 변수로 합을 구하고 ref에 한 번에 값을 넘기는 것과 ref에 계속 값을 더하는 것의 속도 차이는 꽤 심하다. (임계 영역이랑 별개로)
	- single_sum(): 5.519ms   
	multi_sum1(): 1.8216ms   
	multi_sum2(): 8.3963ms
	- 임계 영역이 전혀 없는 코드이고, 후자의 경우엔 싱글 코어보다 느린 것을 확인할 수 있다. 
	- 물론 연산 자체가 매우 단순해서 더 복잡한 연산을 가지고 컴파일러 최적화를 켜서 실행하면 달라질지도 모른다.

3. atomic은 임계 영역이 아예 없는 코드만큼 빠르지는 않다. 하지만 mutex로 직접 락을 거는 것보다는 빠르다.
	- single_sum(): 5.5009ms   
	multi_sum(): 1.8904ms   
	multi_sum_atomic(): 75.8476ms   
	multi_sum_mutex(): 221.948ms

4. atomic의 memory_order의 경우, atomic변수를 하나만 쓸 때는 속도 차이가 없다.
	- multi_sum_atomic_relaxed(): 76.0443ms   
	multi_sum_atomic_seq_cst(): 77.9324ms
	- 여러 개의 atomic 변수를 사용해야 서로간의 memory_order가 적용되어 유의미해지는 것 같다.


## c++ 새로 알게 된 문법

[Generalized lambda capture in C++14](https://stackoverflow.com/questions/8640393/move-capture-in-lambda)