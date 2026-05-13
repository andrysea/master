-- PREPARAZIONE DATI
-- Creazione del database
CREATE DATABASE esame;
USE esame;

-- Creazione della tabella prodotto
CREATE TABLE prodotto (
    id INT PRIMARY KEY,
    nome VARCHAR(100) NOT NULL,
    prezzo DECIMAL(10, 2) NOT NULL
);

-- Creazione della tabella cliente
CREATE TABLE cliente (
    id INT PRIMARY KEY,
    email VARCHAR(100) NOT NULL UNIQUE
);

-- Creazione della tabella ordini
CREATE TABLE ordini (
    id_prodotto INT,
    id_cliente INT,
    data DATE,
    FOREIGN KEY (id_prodotto) REFERENCES prodotto(id),
    FOREIGN KEY (id_cliente) REFERENCES cliente(id)
);

-- Inserimento dei dati nella tabella prodotto
INSERT INTO prodotto (id, nome, prezzo) VALUES
    (1, 'Smartphone', 699.99),
    (2, 'Laptop', 1099.99),
    (3, 'Auricolari Bluetooth', 49.99),
    (4, 'Monitor 4K', 299.99),
    (5, 'Mouse Wireless', 19.99),
    (6, 'Tastiera Meccanica', 89.99),
    (7, 'Tablet', 499.99),
    (8, 'Smartwatch', 199.99),
    (9, 'Fotocamera DSLR', 799.99),
    (10, 'Stampante', 129.99);

-- Inserimento dei dati nella tabella cliente
INSERT INTO cliente (id, email) VALUES
    (1, 'mario.rossi@example.com'),
    (2, 'luigi.verdi@example.com'),
    (3, 'anna.bianchi@example.com'),
    (4, 'carla.neri@example.com'),
    (5, 'giulia.ferri@example.com');

-- Inserimento dei dati nella tabella ordini
INSERT INTO ordini (id_prodotto, id_cliente, data) VALUES
    (1, 1, '2025-01-01'),
    (2, 1, '2025-01-02'),
    (3, 2, '2025-01-03'),
    (4, 3, '2025-01-04'),
    (5, 4, '2025-01-05'),
    (6, 5, '2025-01-06'),
    (7, 1, '2025-01-07'),
    (8, 2, '2025-01-08'),
    (9, 3, '2025-01-09'),
    (10, 4, '2025-01-10'),
    (1, 5, '2025-01-11'),
    (2, 1, '2025-01-12'),
    (3, 2, '2025-01-13'),
    (4, 3, '2025-01-14'),
    (5, 4, '2025-01-15'),
    (6, 5, '2025-01-16'),
    (7, 1, '2025-01-17'),
    (8, 2, '2025-01-18'),
    (9, 3, '2025-01-19'),
    (10, 4, '2025-01-20');

-- Dataset integrato ordini-clienti-prodotti
SELECT
    c.id AS id_cliente,
    c.email AS email_cliente,
    p.id AS id_prodotto,
    p.nome AS nome_prodotto,
    p.prezzo AS prezzo_prodotto,
    o.data AS data_ordine
FROM ordini AS o
INNER JOIN cliente AS c
    ON o.id_cliente = c.id
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id;

-- ANALISI DEI CLIENTI
-- Calcolare la spesa totale per ogni cliente sommando i prezzi dei prodotti acquistati
SELECT
    c.id AS id_cliente,
    c.email AS email_cliente,
    SUM(p.prezzo) AS spesa_totale
FROM ordini AS o
INNER JOIN cliente AS c
    ON o.id_cliente = c.id
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id
GROUP BY
    c.id,
    c.email;

-- Ordinare i clienti in base alla spesa totale e identificare i top spender
-- Ho scelto ordine decrescente per i top spender
SELECT
    c.id AS id_cliente,
    c.email AS email_cliente,
    SUM(p.prezzo) AS spesa_totale
FROM ordini AS o
INNER JOIN cliente AS c
    ON o.id_cliente = c.id
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id
GROUP BY
    c.id,
    c.email
ORDER BY
    spesa_totale DESC;

-- ANALISI DEI PRODOTTI
-- Determinare il fatturato totale generato da ciascun prodotto
SELECT
    p.id AS id_prodotto,
    p.nome AS nome_prodotto,
    SUM(p.prezzo) AS fatturato_totale
FROM ordini AS o
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id
GROUP BY
    p.id,
    p.nome;

-- Ordinare i prodotti in base al fatturato totale per identificare i best-seller
-- Ho scelto ordine decrescente per i best-seller
SELECT
    p.id AS id_prodotto,
    p.nome AS nome_prodotto,
    SUM(p.prezzo) AS fatturato_totale
FROM ordini AS o
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id
GROUP BY
    p.id,
    p.nome
ORDER BY
    fatturato_totale DESC;

-- ANALISI TEMPORALE
-- Aggregare i dati delle vendite per giorno della settimana
SELECT
    WEEKDAY(o.data) AS numero_giorno,
    DAYNAME(o.data) AS giorno_settimana,
    COUNT(o.id_prodotto) AS numero_vendite,
    SUM(p.prezzo) AS fatturato_totale
FROM ordini AS o
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id
GROUP BY
    WEEKDAY(o.data),
    DAYNAME(o.data)
ORDER BY
    numero_giorno;

-- Calcolare il fatturato medio giornaliero e individuare i giorni con la media più alta
SELECT
    WEEKDAY(o.data) AS numero_giorno,
    DAYNAME(o.data) AS giorno_settimana,
    ROUND(SUM(p.prezzo) / COUNT(DISTINCT o.data), 2) AS fatturato_medio_giornaliero
FROM ordini AS o
INNER JOIN prodotto AS p
    ON o.id_prodotto = p.id
GROUP BY
    WEEKDAY(o.data),
    DAYNAME(o.data)
ORDER BY
    fatturato_medio_giornaliero DESC;

-- ANALISI DEGLI ORDINI
-- Calcolare il numero di prodotti per ciascun ordine
-- Poiché la tabella ordini non contiene un id_ordine, considero un ordine come l’insieme degli acquisti effettuati dallo stesso cliente nella stessa data
SELECT
    o.id_cliente AS id_cliente,
    o.data AS data_ordine,
    COUNT(o.id_prodotto) AS numero_prodotti
FROM ordini AS o
GROUP BY
    o.id_cliente,
    o.data
ORDER BY
    o.id_cliente,
    o.data;

-- Determinare la media e la distribuzione del numero di prodotti per ordine
SELECT
    prodotti_per_ordine.numero_prodotti AS numero_prodotti,
    COUNT(*) AS numero_ordini,
    ROUND(
        COUNT(*) * 100.0 / SUM(COUNT(*)) OVER (),
        2
    ) AS percentuale_ordini,
    ROUND(
        SUM(prodotti_per_ordine.numero_prodotti * COUNT(*)) OVER () 
        / SUM(COUNT(*)) OVER (),
        2
    ) AS media_prodotti_per_ordine
FROM (
    SELECT
        o.id_cliente AS id_cliente,
        o.data AS data_ordine,
        COUNT(o.id_prodotto) AS numero_prodotti
    FROM ordini AS o
    GROUP BY
        o.id_cliente,
        o.data
) AS prodotti_per_ordine
GROUP BY
    prodotti_per_ordine.numero_prodotti
ORDER BY
    prodotti_per_ordine.numero_prodotti;
