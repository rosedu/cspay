--
-- Dumping data for table `utilizatori`
--

INSERT INTO `utilizatori` (`materie`, `utilizator`, `parola`, `tip_cont`, `link_univ`) VALUES
('Sisteme de Operare', 'admin', 'admin', 1, 1);

-- 
-- Dumping data for table `universitati`
-- 

INSERT INTO `universitati` (`nume`, `data_start`, `data_stop`) VALUES
('Universitatea Politehnica Bucuresti', '2009-02-23', '2009-06-05');

--
-- Dumping data for table `vacante`
-- 

INSERT INTO `vacante` (`link_univ`, `data_start`, `data_stop`) VALUES 
(1, '2009-04-20', '2009-04-26');

--
-- Dumping data for table `facultati`
--

INSERT INTO `facultati` (`link_univ`, `nume`, `nume_scurt`, `decan`) VALUES
(1, 'Facultatea de Automatica si Calculatoare', 'AC', 'Dumitru Popescu'),
(1, 'Facultatea de Inginerie cu Predare in Limbi Straine', 'FILS', 'Adrian Volceanov');

-- 
-- Dumping data for table `catedre`
-- 

INSERT INTO `catedre` (`link_fac`, `nume`, `sef`) VALUES 
(1, 'Automatica si Informatica Industriala', 'Radu Dobrescu'),
(1, 'Automatica si Ingineria Sistemelor', 'Ioan Dumitrache'),
(1, 'Calculatoare', 'Valentin Cristea');

-- 
-- Dumping data for table `discipline`
--

INSERT INTO `discipline` (`link_fac`, `link_cat`, `nume`, `nume_scurt`) VALUES
(1, 3, 'Utilizarea Sistemelor de Operare', 'USO'),
(1, 3, 'Programarea Calculatoarelor', 'Prog'),
(1, 3, 'Introducere in Informatica', 'II'),
(1, 3, 'Programare Logica', 'PL'),
(1, 3, 'Structuri de Date', 'SD'),
(1, 3, 'Metode Numerice', 'MN'),
(1, 3, 'Programarea in Limbaj de Asamblare', 'PLAS'),
(1, 3, 'Analiza Algoritmilor', 'AA'),
(1, 3, 'Elemente de Electronica Analogica', 'EEA'),
(1, 3, 'Programarea Orientata Obiect', 'POO'),
(1, 3, 'Protocoale de Comunicatii', 'PC'),
(1, 3, 'Proiectarea Algoritmilor', 'PA'),
(1, 3, 'Paradigme de Programare', 'PP'),
(1, 3, 'Electronica Digitala', 'ED'),
(1, 3, 'Calculatoare Numerice 1', 'CN1'),
(1, 3, 'Algoritmi de Prelucrare Distribuita', 'APD'),
(1, 3, 'Retele Locale', 'RL'),
(1, 3, 'Calculatoare Numerice 2', 'CN2'),
(1, 3, 'Limbaje Formale si Automate', 'LFA'),
(1, 3, 'Elemente de Grafica pe Calculator', 'EGC'),
(1, 3, 'Sisteme de Operare', 'SO'),
(1, 3, 'Arhitectura Sistemelor de Calcul', 'ASC'),
(1, 3, 'Proiectarea cu Microprocesoare', 'PM'),
(1, 3, 'Baze de Date', 'BD'),
(1, 3, 'Ingineria Programarii', 'IP'),
(1, 3, 'Ingineria Calculatoarelor', 'IC'),
(1, 3, 'Arhitecturi de Prelucrare Paralela', 'APP'),
(1, 3, 'Proiectarea Retelelor', 'PR'),
(1, 3, 'Sisteme Multiprocesor', 'SMP'),
(1, 3, 'Proiectarea VLSI', 'PVLSI'),
(1, 3, 'Sisteme cu Microprocesoare', 'SM'),
(1, 3, 'Sisteme Incorporate', 'SI'),
(1, 3, 'Procesarea Semnalelor', 'PS'),
(1, 3, 'Testarea Sistemelor de Calcul', 'TSC'),
(1, 3, 'Sisteme de Programe pentru Retele de Calculatoare', 'SPRC'),
(1, 3, 'Compilatoare', 'CPL'),
(1, 3, 'Baze de Date 2', 'BD2'),
(1, 3, 'Sisteme de Operare 2', 'SO2');

-- 
-- Dumping data for table `titulari`
--

INSERT INTO `titulari` (`link_disc`, `nume`, `an`, `serie`) VALUES
(1, 'Razvan Rughinis', 1, 'CA'),
(1, 'Alex Herisanu', 1, 'CB'),
(1, 'Razvan Deaconescu', 1, 'CC'),
(2, 'Valeriu Iorga', 1, 'CA'),
(2, 'Eugenia Kalisz', 1, 'CB'),
(2, 'Carmen Odubasteanu', 1, 'CC'),
(3, 'Francisc Iacob', 1, 'CA'),
(3, 'Francisc Iacob', 1, 'CB'),
(3, 'Francisc Iacob', 1, 'CC'),
(4, 'Serban Petrescu', 1, 'CA'),
(4, 'Mariana Mocanu', 1, 'CB'),
(4, 'Ioan Bucur', 1, 'CC'),
(5, 'Valeriu Iorga', 1, 'CA'),
(5, 'Eugenia Kalisz', 1, 'CB'),
(5, 'Florian Moraru', 1, 'CC'),
(6, 'Valeriu Iorga', 1, 'CA'),
(6, 'Valeriu Iorga', 1, 'CB'),
(6, 'Florin Pop', 1, 'CC'),
(7, 'Vasile Lungu', 2, 'CA'),
(7, 'Vasile Lungu', 2, 'CB'),
(7, 'Vasile Lungu', 2, 'CC'),
(8, 'Cristian Giumale', 2, 'CA'),
(8, 'Stefan Trausan', 2, 'CB'),
(8, 'Andrei Mogos', 2, 'CC'),
(9, 'Nicolae Cupcea', 2, 'CA'),
(9, 'Nicolae Cupcea', 2, 'CB'),
(9, 'Nicolae Cupcea', 2, 'CC'),
(10, 'Florian Moraru', 2, 'CA'),
(10, 'Lorina Negreanu', 2, 'CB'),
(10, 'Carmen Odubasteanu', 2, 'CC'),
(11, 'Valentin Cristea', 2, 'CA'),
(11, 'Gavril Godza', 2, 'CB'),
(11, 'Florin Pop', 2, 'CC'),
(12, 'Vlad Posea', 2, 'CA'),
(12, 'Stefan Trausan', 2, 'CB'),
(12, 'Costin Chiru', 2, 'CC'),
(13, 'Cristian Giumale', 2, 'CA'),
(13, 'Mihaela Balint', 2, 'CB'),
(13, 'Cristian Giumale', 2, 'CC'),
(14, 'Nicolae Cupcea', 2, 'CA'),
(14, 'Nicolae Cupcea', 2, 'CB'),
(14, 'Nicolae Cupcea', 2, 'CC'),
(15, 'Adrian Petrescu', 2, 'CA'),
(15, 'Decebal Popescu', 2, 'CB'),
(15, 'Nirvana Popescu', 2, 'CC'),
(16, 'Valentin Cristea', 3, 'CA'),
(16, 'Alexandru Costan', 3, 'CB'),
(16, 'Ciprian Dobre', 3, 'CC'),
(17, 'Nicolae Tapus', 3, 'CA'),
(17, 'Razvan Rughinis', 3, 'CB'),
(17, 'Razvan Rughinis', 3, 'CC'),
(18, 'Adrian Petrescu', 3, 'CA'),
(18, 'Decebal Popescu', 3, 'CB'),
(18, 'Decebal Popescu', 3, 'CC'),
(19, 'Lorina Negreanu', 3, 'CA'),
(19, 'Lorina Negreanu', 3, 'CB'),
(19, 'Irina Mocanu', 3, 'CC'),
(20, 'Florica Moldoveanu', 3, 'CA'),
(20, 'Irina Mocanu', 3, 'CB'),
(20, 'Marius Zaharia', 3, 'CC'),
(21, 'Octavian Purdila', 3, 'CA'),
(21, 'Marius Zaharia', 3, 'CB'),
(21, 'Andrei Pitis', 3, 'CC'),
(22, 'Nicolae Tapus', 3, 'CA'),
(22, 'Emil Slusanschi', 3, 'CB'),
(22, 'Emil Slusanschi', 3, 'CC'),
(23, 'Nicolae Tapus', 3, 'CA'),
(23, 'Nicolae Tapus', 3, 'CB'),
(23, 'Nicolae Tapus', 3, 'CC'),
(24, 'Mircea Petrescu', 3, 'CA'),
(24, 'Florin Radulescu', 3, 'CB'),
(24, 'Mircea Petrescu', 3, 'CC'),
(25, 'Florica Moldoveanu', 3, 'CA'),
(25, 'Florica Moldoveanu', 3, 'CB'),
(25, 'Alin Moldoveanu', 3, 'CC'),
(26, 'Costin Stefanescu', 3, 'CA'),
(26, 'Costin Stefanescu', 3, 'CB'),
(26, 'Costin Stefanescu', 3, 'CC'),
(38, 'Octavian Purdila', 4, 'C3');
