-- 
-- Dumping data for table `universitati`
-- 

INSERT INTO `universitati` (`univ_id`, `nume`, `data_start`, `data_stop`) VALUES (1, 'Universitatea Politehnica Bucuresti', '2009-02-23', '2008-06-05');

-- 
-- Dumping data for table `vacante`
-- 

-- 
-- Dumping data for table `vacante`
-- 

INSERT INTO `vacante` (`vac_id`, `link_univ`, `data_start`, `data_stop`) VALUES 
(8, 1, '2008-04-28', '2008-05-04');

--
-- Dumping data for table `facultati`
--

INSERT INTO `facultati` (`fac_id`, `link_univ`, `nume`, `nume_scurt`, `decan`) VALUES
(1, 1, 'Facultatea de Automatica si Calculatoare', 'A & C', 'Dumitru Popescu');

-- 
-- Dumping data for table `catedre`
-- 

INSERT INTO `catedre` (`cat_id`, `link_fac`, `nume`, `sef`) VALUES 
(1, 1, 'Automatica si Informatica Industriala', 'Radu Dobrescu'),
(2, 1, 'Automatica si Ingineria Sistemelor', 'Ioan Dumitrache'),
(3, 1, 'Calculatoare', 'Valentin Cristea');

-- 
-- Dumping data for table `discipline`

INSERT INTO `discipline` (`disc_id`, `link_cat`, `nume`, `nume_scurt`) VALUES
(1, 3, 'Utilizarea Sistemelor de Operare', 'USO'),
(2, 3, 'Programarea Calculatoarelor', 'Prog'),
(3, 3, 'Introducere in Informatica', 'II'),
(4, 3, 'Programare Logica', 'PL'),
(5, 3, 'Structuri de Date', 'SD'),
(6, 3, 'Metode Numerice', 'MN'),
(7, 3, 'Programarea in Limbaj de Asamblare', 'PLAS'),
(8, 3, 'Analiza Algoritmilor', 'AA'),
(9, 3, 'Elemente de Electronica Analogica', 'EEA'),
(10, 3, 'Programarea Orientata Obiect', 'POO'),
(11, 3, 'Protocoale de Comunicatii', 'PC'),
(12, 3, 'Proiectarea Algoritmilor', 'PA'),
(13, 3, 'Paradigme de Programare', 'PP'),
(14, 3, 'Electronica Digitala', 'ED'),
(15, 3, 'Calculatoare Numerice 1', 'CN1');

INSERT INTO `titulari` (`tit_id`, `link_disc`, `nume`, `an`, `serie`) VALUES
(1, 1, 'Razvan Rughinis', 1, 'CA'),
(2, 1, 'Alex Herisanu', 1, 'CB'),
(3, 1, 'Razvan Deaconescu', 1, 'CC'),
(4, 2, 'Valeriu Iorga', 1, 'CA'),
(5, 2, 'Eugenia Kalisz', 1, 'CB'),
(6, 2, 'Carmen Odubasteanu', 1, 'CC'),
(7, 3, 'Francisc Iacob', 1, 'CA'),
(8, 3, 'Francisc Iacob', 1, 'CB'),
(9, 3, 'Francisc Iacob', 1, 'CC'),
(10, 4, 'Serban Petrescu', 1, 'CA'),
(11, 4, 'Mariana Mocanu', 1, 'CB'),
(12, 4, 'Ioan Bucur', 1, 'CC'),
(13, 5, 'Valeriu Iorga', 1, 'CA'),
(14, 5, 'Eugenia Kalisz', 1, 'CB'),
(15, 5, 'Florian Moraru', 1, 'CC');
