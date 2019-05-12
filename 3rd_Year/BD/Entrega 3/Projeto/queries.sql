----------------------------------------
-- 1
----------------------------------------

SELECT numprocessosocorro
FROM acciona
GROUP BY numprocessosocorro
HAVING count(*) >= ALL (
    SELECT count(*)
    FROM acciona
    GROUP BY numprocessosocorro
);



----------------------------------------
-- 2
----------------------------------------

SELECT nomeentidade
FROM (
	SELECT distinct numprocessosocorro, nomeentidade
	FROM acciona NATURAL JOIN eventoemergencia
	WHERE instantechamada BETWEEN '2018-06-21 00:00:00' AND '2018-09-23 23:59:59') as t1
GROUP BY nomeentidade
HAVING count(*) >= ALL (
	SELECT count(*)
	FROM (
		SELECT distinct numprocessosocorro, nomeentidade
		FROM acciona NATURAL JOIN eventoemergencia
		WHERE instantechamada BETWEEN '2018-06-21 00:00:00' AND '2018-09-23 23:59:59') as t2
	GROUP BY nomeentidade);



----------------------------------------
-- 3
----------------------------------------

SELECT distinct numprocessosocorro 
FROM (
	SELECT numprocessosocorro, nummeio, nomeentidade
	FROM acciona NATURAL JOIN eventoemergencia 
	WHERE moradalocal = 'Oliveira do Hospital' AND instantechamada BETWEEN '2018-01-01 00:00:00' AND '2018-12-31 23:59:59') AS t1
WHERE NOT EXISTS (
	SELECT 1
	FROM audita
	WHERE numprocessosocorro = t1.numprocessosocorro AND nummeio = t1.nummeio AND nomeentidade = t1.nomeentidade);



----------------------------------------
-- 4
----------------------------------------

SELECT count(*)
FROM segmentovideo NATURAL JOIN video NATURAL JOIN vigia
WHERE duracao > 1 AND moradalocal = 'Monchique' AND datahorainicio BETWEEN '2018-08-01 00:00:00' AND '2018-08-31 23:59:59';



----------------------------------------
-- 5
----------------------------------------

SELECT *
FROM meiocombate
EXCEPT
SELECT nummeio, nomeentidade
FROM meioapoio NATURAL JOIN acciona;



----------------------------------------
-- 6
----------------------------------------

SELECT nomeentidade
FROM meiocombate NATURAL JOIN acciona
GROUP BY nomeentidade
HAVING count(distinct numprocessosocorro) = (
	SELECT count(distinct numprocessosocorro)
	FROM meiocombate NATURAL JOIN acciona);