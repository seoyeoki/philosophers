# 🧪 Philosophers Evaluation Test Suite

이 디렉토리는 **Philosophers 평가(Evaluation) 시 동기화 및 스케줄링의 중요성을 시각적으로 시연**하기 위해 작성된 다양한 비교 테스트 모듈들로 구성되어 있습니다.

각 테스트 빌드는 공통 코드(`main.c`, `init.c`)를 기반으로 하되, 특정 오류나 기능적 차이를 유발하는 소스 파일들을 결합하여 실행 파일을 생성합니다.

---

## 📋 테스트 대상 및 의미

| 실행 파일명 | 테스트 모듈 의미 | 사용되는 주요 변형 파일 |
| :--- | :--- | :--- |
| **`philo_no_mutex_test`** | **뮤텍스가 없는 상태의 위험성 증명**<br>- 공유 데이터(`last_meal_time` 등)에 대한 뮤텍스 락을 제거하여 데이터 레이스 및 다중 사망 출력을 시연합니다. | `monitor_no_mutex.c`<br>`routine_no_mutex.c`<br>`utils_no_mutex.c` |
| **`philo_busy_wait_test`** | **바쁜 대기(Busy Waiting) 시연**<br>- 대기 상태에서 `usleep`이나 락 양보 없이 루프를 돌 때 발생하는 CPU 점유율 증가 및 기동사를 테스트합니다. | `routine_busy_wait.c` |
| **`philo_no_guard_test`** | **출력 보호 가드 누락 시연**<br>- 사망 플래그 확인 시 뮤텍스 가드를 제거하면 철학자 사망 직후에도 다른 행동 로그가 출력되는 현상을 보여줍니다. | `utils_no_guard.c` |
| **`philo_busy_spin_test`** | **Busy Spin Sleep 시연**<br>- 정확한 usleep 대신 루프를 도는 Spin Sleep 방식을 사용했을 때의 스케줄러 노이즈와 자원 낭비를 보여줍니다. | `utils_busy_spin.c` |
| **`philo_usleep_test`** | **기본 usleep 버전**<br>- 기본 usleep 타이머를 활용한 기본 비교 모델입니다. | `routine.c`<br>`utils.c` |
| **`philo_efficient_test`** | **스케줄링 최적화 모델**<br>- 생각 시간 완충 및 그룹 분할 스케줄링이 올바르게 적용된 효율적인 버전입니다. | `routine_efficient.c` |

---

## 🛠️ 실행 및 테스트 방법

### 1. 빌드
`test` 디렉토리 내에서 `make` 명령어를 통해 모든 테스트 바이너리를 빌드합니다.
```bash
make
```

### 2. 평가 시 유용한 데모 시나리오

#### 시나리오 A: 뮤텍스가 없는 경우의 데이터 레이스 시연
1. 데이터 레이스 분석 도구(Thread Sanitizer)를 결합하여 빌드하거나 직접 실행해 봅니다.
   ```bash
   cc -Wall -Wextra -Werror -fsanitize=thread -pthread main.c init.c monitor_no_mutex.c routine_no_mutex.c utils_no_mutex.c -o no_mutex_tsan
   ./no_mutex_tsan 5 800 200 200 7
   ```
2. **관찰 포인트**: 실행하자마자 터미널에 수많은 `WARNING: ThreadSanitizer: data race` 메시지가 쏟아져 나오며, 데이터 동기화 락의 부재를 확실하게 증명할 수 있습니다.

#### 시나리오 B: 바쁜 대기(Busy Wait)로 인한 자원 고갈 시연
1. `philo_busy_wait_test`를 실행합니다.
   ```bash
   ./philo_busy_wait_test 4 310 200 100
   ```
2. 다른 터미널 창에서 CPU 점유율을 확인해 봅니다.
   ```bash
   top -o cpu  # 또는 macOS 활성 상태 보기(Activity Monitor) 확인
   ```
3. **관찰 포인트**: 정상 버전(`philo`)에 비해 CPU 사용률이 매우 크게 치솟는 것을 시각적으로 대조하여 보여줄 수 있습니다.

---

## 🧹 빌드 파일 정리
빌드를 깨끗이 지우려면 아래 명령어를 실행하십시오.
```bash
make fclean
```
