# 프로젝트 목적

- **해당 교과목명**: 데이터베이스시스템
- 이 프로젝트의 목적은 CentOS 7 환경에서 MySQL을 사용하는 데이터베이스 시스템을 C++을 활용하여 개발하고 연습하는 데에 있습니다. 
- MySQL의 C++ API를 활용하여 데이터베이스와의 통신을 구현
- Docker를 이용해 타깃 환경인 Centos7 에서 이용 가능한 빌드 환경을 구축합니다. 

# Requirements

프로젝트를 빌드하고 실행하기 위해 다음 사항을 충족해야 합니다:
- gcc
```bash
gcc version 13.2.0
# 더 높은 버전 또한 지원하며, 더 낮은 버전에 대해서는 확인되지 않았습니다.
```

- OS
```bash
Ubuntu 24.04.1 LTS
# OS가 다를 시 MySQL 에 대한 라이브러리만 수정해주면 됩니다.
```

# 빌드 방법

## Prerequirements
- Connector/C++ (MySQL API Library)
```bash
sudo apt install libmysqlcppconn-dev
```
- GCC(g++) / Make
```bash
sudo apt install build-essential
```

이 프로젝트는 Makefile을 사용하여 빌드됩니다. 다음 단계를 따라 빌드하고 실행할 수 있습니다:

1. 저장소를 클론하여 로컬 환경에 다운로드합니다:
   ```bash
   git clone https://github.com/sony-string/mysql-cpp-practice
   cd mysql-cpp-practice
   ```

2. 프로젝트를 빌드합니다:
   컨테이너 내에서 다음 명령어를 실행하여 프로젝트를 빌드합니다.
   ```bash
   make -j8
   ```
  - 컴파일 옵션을 지정할 수 있습니다.
  ```bash
  make -j8 ADD="원하는 컴파일 옵션" 
  ```

3. (향후) 유닛 테스트:
   현재 유닛 테스트는 통합되지 않았으나, 추가될 예정입니다. 추가되면 다음 명령어로 테스트를 수행할 수 있습니다.
   ```bash
   make test -j8
   ```

# DB 초기화
`db_scripts/club_init.sql` 을 MySQL 에서 실행하면 됩니다.
```bash
mysql -u root -p < db/scripts/club_init.sql
```

# 실행 방법

우선 DB 서버에 대한 정보를 환경 변수로 등록하여야 합니다.
아래의 환경 변수를 입력하시면 됩니다.
```bash
export "MYSQL_SERVER"="MySQL 서버 주소:포트 번호"
export "MYSQL_USER"="MySQL 접속 username"
export "MYSQL_PASSWORD"="MySQL 접속 password"
export "MYSQL_DATABASE"="접속할 MySQL 서버에서 사용할 database 이름"

```
이제 실행할 수 있습니다.
```bash
./sev
```