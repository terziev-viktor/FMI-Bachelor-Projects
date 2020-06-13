CREATE TABLE Staff ( 
egn VARCHAR(10) NOT NULL,
phone VARCHAR(13) ,
dep VARCHAR(120) NOT NULL,
bd DATE,
salary DOUBLE NOT NULL,
linear_manager VARCHAR(10),
PRIMARY KEY (egn));

-- Will act as a null value or default admin
INSERT INTO STAFF (EGN, PHONE, DEP, BD, SALARY, LINEAR_MANAGER ) VALUES ('0000000000', '0000000000', 'Default', '0001-01-01', 00.00, '0000000000');

CREATE TABLE Zones( 
zone_id INT NOT NULL GENERATED ALWAYS AS IDENTITY (START WITH 1 INCREMENT BY 1),
zone_floor INT NOT NULL,
zone_size DOUBLE,
PRIMARY KEY (zone_id));

CREATE TABLE Dep (
name VARCHAR(120) NOT NULL,
office INT,
manager VARCHAR(10) NOT NULL,
PRIMARY KEY (name));

INSERT INTO Dep (name, manager) VALUES('Default', '0000000000');

-- select * from dep;

CREATE TABLE Owners( 
owner_id INT NOT NULL GENERATED ALWAYS AS IDENTITY (START WITH 1 INCREMENT BY 1),
name VARCHAR(250) NOT NULL,
phone VARCHAR(13),
email VARCHAR(120),
PRIMARY KEY (owner_id) );

CREATE TABLE Contracts( 
contracts_id INT NOT NULL GENERATED ALWAYS AS IDENTITY (START WITH 1 INCREMENT BY 1),
price DOUBLE NOT NULL,
owner INT NOT NULL,
--FOREIGN KEY REFERENCES Owners(owner_id),
 zone_id INT NOT NULL,
-- FOREIGN KEY REFERENCES Zones(zones_id),
 stdate DATE,
endate DATE,
PRIMARY KEY (contracts_id));

ALTER TABLE STAFF FOREIGN KEY (dep) REFERENCES DEP (NAME) ON
UPDATE
	NO ACTION ON
	DELETE
	CASCADE;

ALTER TABLE OWNERS ADD CONSTRAINT EMAIL CHECK (EMAIL LIKE '%@%.%');

ALTER TABLE OWNERS ADD CONSTRAINT PHONE CHECK ( 
(CASE
  WHEN LENGTH(RTRIM(TRANSLATE(PHONE, '*', ' 0123456789'))) = 0 
  THEN 1
  ELSE 2
END) = 1 OR PHONE = NULL);

ALTER TABLE STAFF ADD CONSTRAINT PHONE CHECK ( 
(CASE
  WHEN LENGTH(RTRIM(TRANSLATE(PHONE, '*', ' 0123456789'))) = 0 
  THEN 1
  ELSE 2
END) = 1 OR PHONE = NULL);

ALTER TABLE STAFF ADD CONSTRAINT SALARY CHECK (SALARY >= 0.0);

ALTER TABLE STAFF FOREIGN KEY (linear_manager) REFERENCES STAFF (egn);

ALTER TABLE DEP FOREIGN KEY (office) REFERENCES ZONES (zone_id);

ALTER TABLE DEP FOREIGN KEY (manager) REFERENCES STAFF (egn);

ALTER TABLE CONTRACTS FOREIGN KEY (owner) REFERENCES OWNERS (owner_id);

ALTER TABLE CONTRACTS FOREIGN KEY (zone_id) REFERENCES ZONES (zone_id);

CREATE FUNCTION STAFF_IN_DEP (DEPNAME VARCHAR(120))
     RETURNS TABLE (EGN VARCHAR(10), SALARY DOUBLE)
     LANGUAGE SQL
     READS SQL DATA
     NO EXTERNAL ACTION
     DETERMINISTIC
     RETURN
       SELECT EGN, SALARY
         FROM (SELECT * FROM STAFF RIGHT JOIN DEP ON STAFF.DEP = NAME) AS SD
         WHERE SD.NAME = DEPNAME;
        
CREATE FUNCTION MANAGER_OF(DEP_NAME VARCHAR(120))
	RETURNS VARCHAR(120)
	SPECIFIC MANAGER_OF
	LANGUAGE SQL
	READS SQL DATA
	NO EXTERNAL ACTION
	DETERMINISTIC
	BEGIN ATOMIC
	DECLARE THE_MANAGER VARCHAR(10);
	SET THE_MANAGER = (SELECT MANAGER FROM DEP WHERE DEP.NAME = DEP_NAME);
	RETURN THE_MANAGER;
	END

-- ADDS NEW EMPLOYEE TO TABLE STAFF, AUTOMATICALLY LINKING THE MANAGER TO MANAGER OF <THE_DEP>
 CREATE FUNCTION ADD_EMPLOYEE(THE_EGN varchar(10), THE_PHONE VARCHAR(13) , THE_DEP VARCHAR(120), THE_DB DATE, THE_SALARY DOUBLE)
 	RETURNS TABLE (EGN VARCHAR(10),
                   SALARY DOUBLE,
                   EGN_MANAGER VARCHAR(10))	
 	LANGUAGE SQL
 	MODIFIES SQL DATA
 	NO EXTERNAL ACTION
    DETERMINISTIC
 	F1: BEGIN ATOMIC
	 	DECLARE DEP_EXISTS INTEGER;
	 	
	 	SET DEP_EXISTS = (SELECT COUNT(*) FROM DEP WHERE DEP.NAME = THE_DEP);
	 	
	 	IF DEP_EXISTS = 0 THEN
	 		INSERT INTO DEP (NAME, MANAGER, OFFICE) VALUES (THE_DEP, '0000000000', NULL);
	 	END IF;
	 	INSERT INTO STAFF(EGN, PHONE, DEP, BD, SALARY, LINEAR_MANAGER) VALUES (THE_EGN, THE_PHONE, THE_DEP, THE_SALARY, MANAGER_OF(THE_DEP));
 	END;
	RETURN SELECT EGN, PHONE, LINEAR_MANAGER FROM EMPLOYEE WHERE EGN = THE_EGN;
 	END
 	
CREATE FUNCTION SET_DEP_OF(EMPL_EGN VARCHAR(10), DEP_NAME VARCHAR(120))
RETURNS BOOLEAN
     LANGUAGE SQL
     MODIFIES SQL DATA
     NO EXTERNAL ACTION
     BEGIN
       UPDATE STAFF s SET s.DEP = DEP_NAME WHERE s.EGN = EMPL_EGN;
      RETURN TRUE;
     END;

-- Parking spots          
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (0, 20.00);

INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (1, 120.22);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (1, 120.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (1, 60.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (2, 300.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (2, 222.22);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (2, 166.69);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (3, 100.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (3, 100.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (3, 100.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (3, 50.00);
INSERT INTO ZONES (ZONE_FLOOR , ZONE_SIZE) VALUES (3, 50.00);

INSERT INTO OWNERS (NAME, PHONE, EMAIL ) VALUES ('Billa', '1234567890', 'billamanager@billa.com');
INSERT INTO OWNERS (NAME, PHONE, EMAIL ) VALUES ('Lilly', '2455342452', 'lillydrugs@lilly.com');
INSERT INTO OWNERS (NAME, PHONE, EMAIL ) VALUES ('DB', '3523424213', 'dbmanagement@dm.com');
INSERT INTO OWNERS (NAME, PHONE, EMAIL ) VALUES ('fibank', '3453253523', 'fibankmanager@fibank.com');
INSERT INTO OWNERS (NAME, PHONE, EMAIL ) VALUES ('CROP', '4206942069', 'CROP@dope.com');

INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('1234567890', 'Default', 888.888, '0000000000');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('2312567543', 'Default', 888.888, '0000000000');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('1432521356', 'Default', 888.888, '2312567543');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('3464532445', 'Default', 888.888, '2312567543');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('3654745543', 'Default', 888.888, '2312567543');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('3865908675', 'Default', 888.888, '2312567543');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('3242643789', 'Default', 888.888, '2312567543');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('3429765445', 'Default', 888.888, '3242643789');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('4143689675', 'Default', 888.888, '3242643789');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('3245464324', 'Default', 888.888, '3242643789');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('1355478543', 'Default', 888.888, '3242643789');
INSERT INTO STAFF (EGN, DEP, SALARY, linear_manager ) VALUES ('6798654235', 'Default', 888.888, '3242643789');

INSERT INTO DEP (NAME, MANAGER, OFFICE) VALUES ('Cleaning', '2312567543', NULL);
INSERT INTO DEP (NAME, MANAGER, OFFICE ) VALUES ('IT', '3429765445', 3);
INSERT INTO DEP (NAME , MANAGER , OFFICE ) VALUES ('Sales', '0000000000', 9);
INSERT INTO DEP (NAME , MANAGER , OFFICE ) VALUES ('Security', '0000000000', 21);

INSERT INTO CONTRACTS (price, owner, zone_id, stdate, endate) VALUES (1001.99, 1, 2, CURRENT timestamp, CURRENT timestamp + 1 YEAR);
INSERT INTO CONTRACTS (price, owner, zone_id, stdate, endate) VALUES (1001.99, 2, 4, CURRENT timestamp, CURRENT timestamp + 2 YEAR);
INSERT INTO CONTRACTS (price, owner, zone_id, stdate, endate) VALUES (500.99, 3, 6, CURRENT timestamp, CURRENT timestamp + 3 YEAR);

CREATE VIEW OWNERS_ZONES_RENTED 
AS SELECT OWNERS.NAME, COUNT(*) ZONES_RENTED, SUM(PRICE) TOTAL_RENT FROM CONTRACTS LEFT JOIN OWNERS ON contracts.OWNER = owners.OWNER_ID GROUP BY OWNERS.NAME;

CREATE VIEW STAFF_BY_DEP AS
SELECT s.DEP, COUNT(*) STAFF_COUNT FROM STAFF s GROUP BY s.DEP;

SELECT * FROM OWNERS_ZONES_RENTED;
SELECT * FROM STAFF_BY_DEP;
