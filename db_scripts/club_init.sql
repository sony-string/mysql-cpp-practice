/* root 계정으로 접속, club 데이터베이스 생성, club 계정 생성 */
DROP DATABASE IF EXISTS  club;
DROP USER IF EXISTS  club@localhost;
create user club@localhost identified WITH mysql_native_password  by 'club';
create database club;
grant all privileges on club.* to club@localhost with grant option;
commit;

/* club DB 자료 생성 */
 
USE club;

-- Professor 테이블
CREATE TABLE Professor (
    prof_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL
);

-- Club 테이블
CREATE TABLE Club (
    club_id INT PRIMARY KEY AUTO_INCREMENT,
    club_name VARCHAR(100) UNIQUE NOT NULL,
    budget DECIMAL(10, 2) NOT NULL,
    prof_id INT UNIQUE,
    FOREIGN KEY (prof_id) REFERENCES Professor(prof_id) ON DELETE SET NULL
);

-- Location
CREATE TABLE Location (
    loc_id INT PRIMARY KEY AUTO_INCREMENT,
    club_id INT,
    loc_name VARCHAR(100) UNIQUE NOT NULL,
    FOREIGN KEY (club_id) REFERENCES Club(club_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Student 테이블
CREATE TABLE Student (
    student_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    department VARCHAR(100) NOT NULL
);

-- Equipment 테이블
CREATE TABLE Equipment (
    equip_id INT PRIMARY KEY AUTO_INCREMENT,
    equip_name VARCHAR(100) NOT NULL
);

-- Activity 테이블
CREATE TABLE Activity (
    act_id INT PRIMARY KEY AUTO_INCREMENT,
    club_id INT,
    act_title VARCHAR(255) NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE,
    FOREIGN KEY (club_id) REFERENCES Club(club_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Gathering 테이블
CREATE TABLE Gathering (
    gathering_id INT PRIMARY KEY AUTO_INCREMENT,
    act_id INT UNIQUE,
    gathering_name VARCHAR(255) NOT NULL,
    FOREIGN KEY (act_id) REFERENCES Activity(act_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Result 테이블
CREATE TABLE Result (
    result_id INT PRIMARY KEY AUTO_INCREMENT,
    club_id INT,
    year YEAR NOT NULL,
    FOREIGN KEY (club_id) REFERENCES Club(club_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Club과 Student의 다대다 관계
CREATE TABLE Club_Student (
    club_id INT,
    student_id INT,
    PRIMARY KEY (club_id, student_id),
    FOREIGN KEY (club_id) REFERENCES Club(club_id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (student_id) REFERENCES Student(student_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Gathering과 Student의 다대다 관계
CREATE TABLE Gathering_Student (
    gathering_id INT,
    student_id INT,
    PRIMARY KEY (gathering_id, student_id),
    FOREIGN KEY (gathering_id) REFERENCES Gathering(gathering_id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (student_id) REFERENCES Student(student_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Activity와 Result의 다대다 관계를 위한 중간 테이블
CREATE TABLE Result_Activity (
    result_id INT,
    act_id INT,
    PRIMARY KEY (result_id, act_id),
    FOREIGN KEY (result_id) REFERENCES Result(result_id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (act_id) REFERENCES Activity(act_id) ON DELETE CASCADE ON UPDATE CASCADE
);

-- Club과 Equipment의 관계 (1:N)
CREATE TABLE Club_Equipment (
    club_id INT,
    equip_id INT,
    PRIMARY KEY (club_id, equip_id),
    FOREIGN KEY (club_id) REFERENCES Club(club_id) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (equip_id) REFERENCES Equipment(equip_id) ON DELETE CASCADE ON UPDATE CASCADE
);
