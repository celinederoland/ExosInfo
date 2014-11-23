;; PROJET SYSTEME EXPERT INFO527
;; Mickaël Koza
;; Morgan Durand
;; Céline de Roland
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; BASE DE FAIT
;; On définit la structure étudiant
;; On définit les listes, les paramètres de notre système (nombre de places) et les variables temporaires dont on a besoin
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;Structure etudiant
  (defstruct etudiant
  (prenom nil)
  (nom nil)
  (lettre_recommandation nil)
  (cv nil)
  (experience_pro nil)
  (erasmus nil)
  (francophone nil)
  (prise_contact nil)
  (renseigne nil)
  (nb_points 0)
  (niveau_etude nil)
  (distance nil)
  (moyenne nil)
  (age 0)
  (nb_redoublement nil)
  (lettre_motivation nil))
 
;;paramètres du système et variables temporaires
(defvar Liste-Etudiants(list nil))
(defvar admis nil)
(defvar attente nil)
(defvar exclus nil)
(defvar nb_places 5)
(defvar tmp nil)
(defvar etu nil)
(defvar iterateur 1)

;;variables temporaires pour la fonction base de fait
(defvar PRENOM)
(defvar NOM)
(defvar AGE)
(defvar CV)
(defvar DISTANCE)
(defvar ERASMUS)
(defvar EXPERIENCE_PRO)
(defvar FRANCOPHONE)
(defvar LETTRE_MOTIVATION)
(defvar LETTRE_RECOMMANDATION)
(defvar MOYENNE)
(defvar NB_REDOUBLEMENT)
(defvar NIVEAU_ETUDE)
(defvar PRISE_CONTACT)
(defvar RENSEIGNE)

;;La fonction base de fait permet à M. Hyvernat de rentrer les informations sur les étudiants candidats
(defun base_de_fait ()
	(print "Entrer le prenom de l'étudiant ( '/'  pour quitter)")
	(setq prenom(read))
	;; tant qu'on ajoute des étudiant
	  (loop while (not ( equal prenom '/))
		do		
			; # debut du remplissage de la base de fait		 
					(print "Entrer le nom de famille de l'utilisateur" )
					(setq nom (read))

					; Niveau d'étude de l'élève
					(print "Quel est le niveau d\'étude de l'élève (exemple L2 : 2) ? " ) 
					(defvar niveau_etude nil) 
					(setq niveau_etude(read))
					 (loop while (> niveau_etude 8)
						do
							(print "doit etre compris entre 0 et 8")
							(setq niveau_etude(read))
					 ) ; end loop

					; Distance entre la fac et d'ou vient l'élève
					(print "A quel distance l'élève habite-t-il de la fac ? " ) 
					(setq distance(read))
					 (loop while (or (< distance 0) (> distance 20000) )
						do
							(print "doit etre compris entre 0 et 20 000")
							(setq distance(read))
					 ) ; end loop

					; Moyenne scolaire de l'élève
					(print "Quelle est la moyenne de l'élève")
					(defvar moyenne nil)
					(setq moyenne(read))
					 (loop while (or (< moyenne 0) (> moyenne 21) )
						do
							(print "doit etre compris entre 0 et 20")
							(setq moyenne(read))
					 ) ; end loop

					; l'age de l'élève
					(print "Quel est l'âge de l'élève")
					(defvar age nil)
					(setq age(read))
					 (loop while (or (< age 0) (> age 60) )
						do
							(print "doit etre compris entre 0 et 60")
							(setq age(read))
					 ) ; end loop

					; Le nombre de redoublement
					(print "Combien de fois l'élève a-t-il reboubler depuis sont obtention du bac ?")
					(defvar nb_redoublement nil)
					(setq nb_redoublement(read))
					 (loop while (or (< nb_redoublement 0) (> nb_redoublement 10) )
						do
							(print "doit etre compris entre 0 et 10")
							(setq nb_redoublement(read))
					 ) ; end loop

					 ; Lettre de motivation
					(print "L'élève à-t-il fourni une lettre de motivation ? [O/n]")
					(defvar lettre_motivation nil)
					(setq lettre_motivation(read))
					 (loop while (not (or(equal lettre_motivation 'o) (equal lettre_motivation 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq lettre_motivation(read))
					 ); end loop
					 (cond ((equal lettre_motivation 'o) (setq lettre_motivation  T))
					 (t (setq lettre_motivation nil))
					 )

					 ; Lettre de recommandation
					(print "L'élève à-t-il fourni une lettre de recommandation de sont professeur principal ? [O/n]")
					(defvar lettre_recommandation nil)
					(setq lettre_recommandation(read))
					 (loop while (not (or(equal lettre_recommandation 'o) (equal lettre_recommandation 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq lettre_recommandation(read))
					 ); end loop
					  (cond ((equal lettre_recommandation 'o) (setq lettre_recommandation  T))
					 (t (setq lettre_recommandation nil))
					 )

					 ; cv
					(print "L'élève à-t-il fourni un CV [O/n]")
					(defvar cv nil)
					(setq cv(read))
					 (loop while (not (or(equal cv 'o) (equal cv 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq cv(read))
					 ); end loop
					 (cond ((equal cv 'o) (setq cv  T))
					 (t (setq cv nil))
					 )

					  ; experience_pro
					(print "L'élève à-t-il deja eu une experience professionnelle ? [O/n]")
					(defvar experience_pro nil)
					(setq experience_pro(read))
					 (loop while (not (or(equal experience_pro 'o) (equal experience_pro 'n)) )
						do
							(print "doit être o (oui) ou n (non)")
							(setq experience_pro(read))
					 ); end loop
					  (cond ((equal experience_pro 'o) (setq experience_pro  T))
					 (t (setq experience_pro nil))
					 )

					 ; erasmus
					(print "L'élève est-il admis dans le cadre d'un programme d'échange international ? (Erasmus) [O/n]")
					(defvar erasmus nil)
					(setq erasmus(read))
					 (loop while (not (or(equal erasmus 'o) (equal erasmus 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq erasmus(read))
					 ); end loop
					  (cond ((equal erasmus 'o) (setq erasmus  T))
					 (t (setq erasmus nil))
					 )

					 ; francophone
					(print "L'élève maitrise t-il bien le français ? (est-il francophone) [O/n]")
					(defvar francophone nil)
					(setq francophone(read))
					 (loop while (not (or(equal francophone 'o) (equal francophone 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq francophone(read))
					 ); end loop
					  (cond ((equal francophone 'o) (setq francophone  T))
					 (t (setq francophone nil))
					 )

					 ; prise_contact
					(print "L'élève à-t-il cherché à entrer en contact avec un responsable de la section ? [O/n]")
					(defvar prise_contact nil)
					(setq prise_contact(read))
					 (loop while (not (or(equal prise_contact 'o) (equal prise_contact 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq prise_contact(read))
					 ); end loop
					 (cond ((equal prise_contact 'o) (setq prise_contact  T))
					 (t (setq prise_contact nil))
					 )

					  ; renseigne
					(print "L'élève a-t-il l'air renseigné sur la section ? [O/n]")
					(defvar renseigne nil)
					(setq renseigne(read))
					 (loop while (not (or(equal renseigne 'o) (equal renseigne 'n)) )
						do
							(print "doit etre o (oui) ou n (non)")
							(setq renseigne(read))
					 ); end loop
					  (cond ((equal renseigne 'o) (setq renseigne  T))
					 (t (setq renseigne nil))
					 )

					;; enregistrement de l'étudiant
					(setf tmp (make-etudiant :prenom prenom :nom nom :niveau_etude niveau_etude  :distance distance :moyenne moyenne :age age
					 :nb_redoublement nb_redoublement  :lettre_motivation lettre_motivation  :lettre_recommandation lettre_recommandation  :cv cv 
					  :experience_pro experience_pro  :erasmus erasmus  :francophone francophone  :prise_contact prise_contact  :renseigne renseigne
					   :nb_points 0))
					 (cond ((equal iterateur 1) 
						(setq iterateur (+ iterateur 1))
						(setq liste-Etudiants (list tmp))
					 )
					 (t 
					 ; on ajoute notre etudiant dans la liste
					 (setq liste-Etudiants(cons tmp Liste-Etudiants))
					 ))

				(print "Entrer le prenom de l'utilisateur ( '/'  pour quitter)")
				(setq prenom(read))
	 ) ; end loop
 ) ; end function
 

(defun renverse (l) ;;la fonction renverse retourne la liste afin que les premiers candidats inscrits soient aussi les premiers dont on étudie la candidature et non l'inverse
	(cond 
		((< (length l) 2) l)
		(t (append (renverse (cdr l)) (list (car l))))
	)
)

;;On appelle la fonction base_de_fait, les réponses sont préenregistrées ci-dessous afin que la démonstration se fasse plus rapidement
(base_de_fait)
A
a
3
50
18
22
0
o
o
o
o
o
o
o
o
B
b
3
45
17
21
1
o
o
o
o
o
o
o
o
C
c
3
10
19.5
20
0
o
o
o
o
o
o
o
o
D
d
3
30
16
24
1
o
o
o
o
o
o
n
o
E
e
3
85
15
22
1
o
o
o
o
o
o
o
o
Fi
fi
2
74
19
21
0
o
o
o
o
o
o
o
o
G
g
2
73
17
23
1
o
o
o
o
o
o
o
o
H
h
1
150
12
25
0
o
o
o
o
o
o
o
o
I
i
1
100
11
35
5
o
o
n
o
n
o
o
n
J
j
2
110
14
23
2
o
o
n
o
n
o
n
o
K
k
2
150
15
21
1
o
n
o
n
o
n
o
n
L
l
2
34
16
21
0
o
o
o
o
o
o
o
o
M
m
2
1000
14
26
1
o
o
o
o
o
o
o
o
/

 

(setq liste-etudiants (renverse Liste-etudiants)) ;;on renverse la liste d'étudiants après l'avoir enregistrée

;;;;;;;;;;;;;;;;;;;;;;
;;  BASE DE REGLES  ;;
;;;;;;;;;;;;;;;;;;;;;;
;; Les règles sont organisées dans l'ordre dans lequel on va les appliquer :
;; D'abord les règles éliminatoires (pas besoin d'étudier les étudiants éliminés)
;; Ensuite les règles les plus importantes, puis les règles moins importantes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defvar Liste_regle 
    '( 
        ( 
            ( < (etudiant-niveau_etude etu) 2) 
            -100
        )
        ( 
            ( < (etudiant-moyenne etu) 8) 
            -100
        )
      ( 
            ( > (etudiant-nb_redoublement etu) 3) 
            -100
        )
        ( 
            ( > (etudiant-moyenne etu) 9) 
            (/ (etudiant-moyenne etu) 2)
        )
        ( 
            ( not 
                (null (etudiant-experience_pro etu))
            ) 
            5
        )
        ( 
            ( > (etudiant-niveau_etude etu) 1) 
            4
        )
        ( 
            ( < (etudiant-distance etu) 200) 
            4
        )
        ( 
            ( not 
                (null (etudiant-lettre_motivation etu))
            ) 
            2
        )
        ( 
            ( not 
                (null (etudiant-lettre_recommandation etu))
            ) 
            2
        )
        ( 
            ( not 
                (null (etudiant-cv etu))
            ) 
            2
        )
        ( 
            ( > (etudiant-age etu) 26) 
            2
        )
        ( 
            ( not 
                (null (etudiant-erasmus etu))
            ) 
            2
        )
        ( 
            ( not 
                (null (etudiant-francophone etu))
            ) 
            2
        )
        ( 
            ( not 
                (null (etudiant-prise_contact etu))
            ) 
            2
        )
        ( 
            ( not 
                (null (etudiant-renseigne etu)) 
            )
            2
        )
    )
)




;;;;;;;;;;;;;;;;;;;;;;
;;      MOTEUR      ;;
;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;TESTER / APPLIQUER les règles
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun applicable (Liste_regle) ;;vérifie la condition de la première règle de la liste (comme on fait une récursion dans le moteur ce n'est pas toujours la même qui est en premier)
    (eval (car (car Liste_regle)))
)

(defun appliquer (Liste_regle) ;;applique la condition de la première règle de la liste (comme on fait une récursion dans le moteur ce n'est pas toujours la même qui est en premier)
	(setf etu 
		(make-etudiant 
			:nom (etudiant-nom etu)
			:prenom (etudiant-prenom etu)
			:lettre_recommandation (etudiant-lettre_recommandation etu) 
			:cv (etudiant-cv etu)
			:experience_pro (etudiant-experience_pro etu) 
			:erasmus (etudiant-erasmus etu) 
			:francophone (etudiant-francophone etu) 
			:prise_contact (etudiant-prise_contact etu) 
			:renseigne (etudiant-renseigne etu) 
			:nb_points (+ 
									(etudiant-nb_points etu) 
										(eval 
											(cadr 
												(car Liste_regle)
											)
										)
									)
			:niveau_etude (etudiant-niveau_etude etu) 
			:distance (etudiant-distance etu) 
			:moyenne (etudiant-moyenne etu) 
			:age (etudiant-age etu) 
			:nb_redoublement (etudiant-nb_redoublement etu) 
			:lettre_motivation (etudiant-lettre_motivation etu)
		)
	)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;Fonctions utilitaires pour modifier un étudiant de la liste d'étudiants ou le supprimer
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun debutn (l n) ;;recupere les premiers éléments d'une liste
	(cond
		((= n 1) NIL)
		( T 
			(cons 
				(car l)
				(debutn (cdr l) (- n 1)) 
			) 
		)
	)
)

(defun finn (l n) ;;recupere les derniers éléments d'une liste
	(cond
		((= n 1) (cdr l))
		( T 
			(cdr 
				(finn l (- n 1))
			)
		)
	)
)

(defun modifiern (n) ;;modifie le nieme élément d'une liste en le remplacant par l'étudiant etu
	(setq Liste-Etudiants 
		(append 
			(debutn Liste-Etudiants n) 
			(cons 
				etu 
				(finn Liste-Etudiants n)
			)
		)
	)
)

(defun supprimer (n) ;;supprime le nieme élément d'une liste
	(setq Liste-Etudiants 
		(append 
			(debutn Liste-Etudiants n) 
			(finn Liste-Etudiants n)
		)
	)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; PLACEMENT DES ETUDIANTS
;; Les étudiants qui atteignent 20 points sont placés en liste d'admis si il y a de la place
;; Les étudiants qui atteignent -100 points sont exclus
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(defun pleine (l) ;;pour voir si il reste de la place sur liste d'admis
	( >= (length l) nb_places )
)

(defun selectionner (n) ;;place l'étudiant en liste d'admis
	(setq admis (cons etu admis))
	(supprimer n)
)

(defun miseattente (n) ;;place l'étudiant en liste d'attente
	(setq attente (cons etu attente))
	(supprimer n)
)

(defun exclure (n) ;;place l'étudiant en liste exclus
	(setq exclus (cons etu exclus))
	(supprimer n)
)

(defun placer (n) ;; décide si un étudiant doit être placé dans une des 3 listes
	(
		cond 
		((and (> (etudiant-nb_points etu) 20) (not (pleine admis))) (selectionner n) (- n 1))
		((and (> (etudiant-nb_points etu) 20) (pleine admis)) (miseattente n) (- n 1))
		((< (etudiant-nb_points etu) -20) (exclure n) (- n 1) )
		(T n)
	)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; OPERATION FINALE
;; Mettre en attente des étudiants ni admis ni exclus
;; Rechercher le meilleur étudiant de la liste d'attente pour le placer dans la liste admis si elle n'est pas pleine
;; Recommencer jusqu'à ce que la liste d'admis soit pleine ou qu'il n'y ait plus d'étudiants en attente avec plus de 15 points
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defvar etu0)
(defun maxi (l n) ;;Recherche du meilleur étudiant, renvoie une liste ( meilleur étudiant, sa position dans la liste d'attente)
	(cond 
		((not l) (list 
			(setf etu0 (make-etudiant :lettre_recommandation T :cv T :experience_pro nil :erasmus nil :francophone T :prise_contact nil 
 :renseigne T :nb_points 0 :niveau_etude 0 :distance 50 :moyenne 13 :age 22 :nb_redoublement 1 :lettre_motivation T))
		 0))
		(t 
			(cond 
				(
					( > (etudiant-nb_points (car l)) (etudiant-nb_points (car (maxi (cdr l) (+ n 1)))) )
					(list (car l) n)
				)
				(
					t
					(maxi (cdr l) (+ n 1))
				)
			)
		)
	)
)

(defvar ltmp)
(defun pts_premier_attente () ;;Renvoie le nombre de points du meilleur étudiant de la liste d'attente (pour voir si il a plus de 15 points)
	(setq ltmp (maxi attente 1))
	(setq etu (car ltmp))
	(etudiant-nb_points etu)
)

(defun supprimer_attente (n) ;;Fonction utilitaire pour supprimer un étudiant de position définie dans la liste d'attente
	(setq attente
		(append 
			(debutn attente n) 
			(finn attente n)
		)
	)
)

(defvar tmp_maxi)
(defun recuperer () ;;Enleve le meilleur étudiant de la liste d'attente pour le placer dans la liste d'admis
	(setq ltmp (maxi attente 1))
	(setq tmp_maxi (cadr ltmp))
	(setq etu (car ltmp))
	(print etu)
	(setq admis (cons etu admis))
	(supprimer_attente tmp_maxi)
)

(defun verif_plein () ;;Si la liste admis n'est pas pleine et qu'il y a des étudiants à plus de 15 points en attente, appelle recuperer et recommence
	(cond
		(
			(and (> (pts_premier_attente) 15) (not (pleine admis)))
			(recuperer)
			(verif_plein)
		)
	)
)

(defun mise_attente_fin () ;;met en liste d'attente les étudiants qui ne sont ni admis ni exclus à la fin
	(setq attente (append attente Liste-etudiants))
	(setq Liste-etudiants nil)
)


;;;;;;;;;;;;;;;;;;;;;;;;;
;; MOTEUR et SOUS MOTEUR
;;;;;;;;;;;;;;;;;;;;;;;;;

(defun sousmoteur (Liste-EtudiantsF Liste-ReglesF n)
	(
		cond
		( (NULL Liste-EtudiantsF) T ) ;; condition d'arrêt du sous moteur
		( T
				(setq etu (car Liste-EtudiantsF))
				(cond
					(
						(applicable Liste-ReglesF)
						(appliquer Liste-ReglesF)
					)
				)
				(modifiern n) ;; remplace l'étudiant numero n de la liste par l'étudiant temporaire etu
				(setq n (placer n)) ;; on place l'étudiant dans la liste qui lui convient (admis, attente, exclus) si son nombre de points l'exige
			(sousmoteur (cdr Liste-EtudiantsF) Liste-ReglesF (+ n 1)) ;; on continue le sous moteur avec l'étudiant suivant
		)
	)
)

(defun moteur (Liste-ReglesF)
	(
		cond
		(
			(NULL Liste-ReglesF) ;; condition d'arrêt du moteur
			(mise_attente_fin) ;; à la fin, on met en attente les étudiants qui n'ont été ni admis ni exclus
			(verif_plein) ;; on complète la liste d'admis si possible avec les étudiants en attente ayant plus de 15 points
		)
		( T
			(sousmoteur Liste-Etudiants Liste-ReglesF 1) ;; on applique le sous moteur pour la règle actuelle sur tous les étudiants
			(moteur (cdr Liste-ReglesF)) ;; on continue le moteur avec la règle suivante
		)
	)	
)


;; EXECUTE
(moteur Liste_regle)
"liste étudiants"
liste-etudiants
"admis"
admis
"liste attente"
attente
"exclus"
exclus
