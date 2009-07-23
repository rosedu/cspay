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
(1, 'Facultatea de Inginerie Electrica', 'IE', 'Claudia Popescu'),
(1, 'Facultatea de Energetica', 'ENG', 'George Darie'),
(1, 'Facultatea de Automatica si Calculatoare', 'AC', 'Dumitru Popescu'),
(1, 'Facultatea de Electronica, Telecomunicatii si Tehnologia Informatiei', 'ETTI', 'Teodor Petrescu'),
(1, 'Facultatea de Inginerie Mecanica si Mecatronica', 'IMM', 'Tudor Prisecaru'),
(1, 'Facultatea de Ingineria si Managementul Sistemelor Tehnologice', 'IMST', 'Gheorghe Amza'),
(1, 'Facultatea de Ingineria Sistemelor Biotehnice', 'ISB', 'Gigel Paraschiv'),
(1, 'Facultatea de Transporturi', 'TSP', 'Corneliu Alexandrescu'),
(1, 'Facultatea de Inginerie Aerospatiala', 'IA', 'Virgil Stanciu'),
(1, 'Facultatea de Stiinta si Ingineria Materialelor', 'SIM', 'Rami Saban'),
(1, 'Facultatea de Chimie Aplicata si Stiinta Materialelor', 'CASM', 'Horia Iovu'),
(1, 'Facultatea de Inginerie cu Predare in Limbi Straine', 'FILS', 'Adrian Volceanov'),
(1, 'Facultatea de Stiinte Aplicate', 'SA', 'Constantin Udriste');

-- 
-- Dumping data for table `catedre`
-- 

INSERT INTO `catedre` (`link_fac`, `nume`, `sef`) VALUES 
(3, 'Automatica si Informatica Industriala', 'Radu Dobrescu'),
(3, 'Automatica si Ingineria Sistemelor', 'Ioan Dumitrache'),
(3, 'Calculatoare', 'Valentin Cristea');

-- 
-- Dumping data for table `discipline`
--

INSERT INTO `discipline` (`link_fac`, `link_cat`, `nume`, `nume_scurt`) VALUES
(3, 3, 'Utilizarea Sistemelor de Operare', 'USO'),
(3, 3, 'Programarea Calculatoarelor', 'Prog'),
(3, 3, 'Introducere in Informatica', 'II'),
(3, 3, 'Proiectare Logica', 'PL'),
(3, 3, 'Structuri de Date', 'SD'),
(3, 3, 'Metode Numerice', 'MN'),
(3, 3, 'Introd. in Org. Calc. si L. de Asambl.', 'PLAS'),
(3, 3, 'Analiza Algoritmilor', 'AA'),
(3, 3, 'Elemente de Electronica Analogica', 'EEA'),
(3, 3, 'Programarea Orientata pe Obiecte', 'POO'),
(3, 3, 'Protocoale de Comunicatii', 'PC'),
(3, 3, 'Proiectarea Algoritmilor', 'PA'),
(3, 3, 'Paradigme de Programare', 'PP'),
(3, 3, 'Electronica Digitala', 'ED'),
(3, 3, 'Calculatoare Numerice', 'CN1'),
(3, 3, 'Algoritmi Paraleli si Distribuiti', 'APD'),
(3, 3, 'Retele Locale', 'RL'),
(3, 3, 'Calculatoare Numerice II', 'CN2'),
(3, 3, 'Limbaje Formale si Automate', 'LFA'),
(3, 3, 'Elemente de Grafica pe Calc.', 'EGC'),
(3, 3, 'Sisteme de Operare', 'SO'),
(3, 3, 'Arhitectura Sistemelor de Calcul', 'ASC'),
(3, 3, 'Proiectarea cu Microprocesoare', 'PM'),
(3, 3, 'Baze de Date', 'BD'),
(3, 3, 'Ingineria Programarii', 'IP'),
(3, 3, 'Ingineria Calculatoarelor', 'IC'),
(3, 3, 'Arhitecturi de Prelucrare Paralela', 'APP'),
(3, 3, 'Proiectarea Retelelor', 'PR'),
(3, 3, 'Sisteme Multiprocesor', 'SMP'),
(3, 3, 'Proiectarea VLSI', 'PVLSI'),
(3, 3, 'Sisteme cu Microprocesoare', 'SM'),
(3, 3, 'Sisteme Incorporate', 'SI'),
(3, 3, 'Procesarea Semnalelor', 'PS'),
(3, 3, 'Testarea Sistemelor de Calcul', 'TSC'),
(3, 3, 'Sisteme de Programe pentru Retele de Calculatoare', 'SPRC'),
(3, 3, 'Compilatoare', 'CPL'),
(3, 3, 'Baze de Date 2', 'BD2'),
(3, 3, 'Sisteme de Operare 2', 'SO2'),
(3, 3, 'Inteligenta Artificiala', 'IA'),
(3, 3, 'Interfata Om-Calculator', 'IOM'),
(3, 3, 'Sisteme de Prelucrare Grafica', 'SPG'),
(3, 3, 'Invatare Automata', 'IA'),
(3, 3, 'Utilizarea Bazelor de Date', 'UBD'),
(3, 3, 'Evaluarea Performantelor', 'EP'),
(3, 3, 'Managementul Proiectelor Software', 'MPS'),
(3, 3, 'Programare Web', 'PW'),
(3, 3, 'Programare Web', 'PW'),
(3, 3, 'Integrarea Sistemelor Informatice', 'ISI'),
(3, 3, 'Sisteme Tolerante la Defecte', 'STD'),
(3, 3, 'Aplicatii Integrate pt. Intreprinderi', 'AII'),
(3, 3, 'E-commerce', 'EC'),
(3, 3, 'E-commerce', 'EC'),
(3, 3, 'Instrumente pt. Dezvoltarea Programelor', 'IDP'),
(3, 3, 'Structura si Arhitectura Sistemelor Numerice', 'SASN'),
(3, 3, 'Sisteme de Prelucr. a Imaginilor', 'SPI'),
(3, 3, 'Algoritmi de Aprox. pt. Probl. NP Complete', 'AAPN'),
(3, 3, 'Sisteme Paralele si Distribuite', 'SPP'),
(3, 3, 'Sisteme Distribuite', 'SD'),
(3, 3, 'Sisteme Avansate pt. Baze de Date', 'SABD'),
(3, 3, 'Prelucrari Grafice', 'PG'),
(3, 3, 'Prelucrari Paralele si Distribuite', 'PPD'),
(3, 3, 'Prelucrari Distribuite in Internet', 'PDI'),
(7, 0, 'Programarea Calculatoarelor', 'PC'),
(13, 0, 'Programarea Calculatoarelor', 'PC'),
(12, 0, 'Computer Graphics', 'CG'),
(12, 0, 'Data Structures and Algorithms', 'DSA'),
(12, 0, 'Functional Programming', 'FP'),
(12, 0, 'Systemes de traitement de l\'infomation', 'STI'),
(12, 0, 'Softwaresysteme', 'SS'),
(12, 0, 'Kompurenetze', 'KN'),
(1, 0, 'Limbaje de Programare', 'LP'),
(1, 0, 'Limbaje de Programare', 'LP'),
(1, 0, 'Limbaje de Programare', 'LP'),
(2, 0, 'Programarea calculatoarelor', 'PC'),
(2, 0, 'Programarea calculatoarelor', 'PC'),
(2, 0, 'Programarea calculatoarelor', 'PC'),
(3, 3, 'Electronica Analogica', 'EA'),
(3, 3, 'Arhitectura Calculatoarelor', 'AC'),
(7, 0, 'Introducere in Informatica', 'II'),
(8, 0, 'Programarea Calculatoarelor', 'PC');

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
(27, 'Nicolae Tapus', 4, 'C1'),
(28, 'Razvan Rughinis', 4, 'C1'),
(29, 'Francisc Iacob', 4, 'C1'),
(30, 'Adrian Petrescu', 4, 'C1'),
(31, 'Cornel Popescu', 'C2'),
(32, 'Dan Tudose', 'C2')
(33, 'Serban Petrescu', 'C2'),
(34, 'Ioan Bucur', 'C2'),
(35, 'Valentin Cristea', 'C3'),
(36, 'Bogdan Nitulescu', 'C3'),
(37, 'Mircea Petrescu', 'C3'),
(38, 'Octavian Purdila', 'C3'),
(39, 'Adina Florea', 'C4'),
(40, 'Stefan Trausan', 'C4'),
(41, 'Florica Moldoveanu', 'C4'),
(42, 'Adina Florea', 'C4'),
(43, 'Florin Radulescu', 'C5'),
(44, 'Dorin Irimescu', 'C5'),
(45, 'Costin Boiangiu', 'C5'),
(46, 'Florin Radulescu', 'C5'),
(47, 'Ciprian Dobre', 'C5'),
(48, 'Mariana Mocanu', 'C5')
(49, 'Costin Stefanescu', 'D'),
(50, 'Alexandru Boicea', 'D'),
(51, 'Nirvana Popescu', 'D'),
(52, 'Ciprian Dobre', 'D'),
(53, 'Ciprian Dobre', 'D'),
(54, 'Cristian Morarescu', 'D');

--
-- Dumping data for table `utilizatori`
--

INSERT INTO `utilizatori` (`utilizator`, `parola`, `tip_cont`, `link_cat`, `link_disc`) VALUES
('secretara', 'secretara', 'S', 3, 0);
