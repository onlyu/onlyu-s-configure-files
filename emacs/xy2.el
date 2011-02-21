; this file is for xy2's emacs mode
(setq xy2-engine-dir "~/dh2/trunk/xy2main/")
(setq xy2-dir "J:/xy2")
(setq xy2-script-dir "J:/xy2/script")
(setq box2d-dir "E:/box2d")

(defun xy2-script()
  "goto xy2 script's directory"
  (interactive)
  (setq tags-file-name "j:/xy2/script/TAGS")
  (find-file xy2-script-dir))


(defun xy2-engine()
  ""
  (interactive)
  (find-file xy2-engine-dir))

(defun goto-box2d()
  ""
  (interactive)
  (find-file box2d-dir))
(defun goto-irrlua()
  "goto irrlicht's script menu"
  (interactive)
  (find-file "E:/irrlicht-1.6.1/source/IrrLua/examples"))
(defun goto-xy3-script()
  ""
  (interactive)
  (find-file "E:/xy3/script"))

(defun goto-fs()
  ""
  (interactive)
  (find-file "/cygdrive/f/fs/trunk/script/"))
(defun goto-xy2()
  "goto xy2's home menu"
  (interactive)
  (setq tags-file-name "j:/xy2/script/TAGS")
  (find-file "j:/xy2/script"))

(defun goto-cool()
  "goto cool's home menu"
  (interactive)
  (find-file "E:/cool/bin/scripts/init/initgame.lua"))

(defun goto-love()
  "goto love's script menu"
  (interactive)
  (find-file "e:/love-0.5-0/bin/game/love/init.lua"))

(defun goto-newxy()
  "goto newxy's home dir"
  (interactive)
  (find-file "E:/newxy/newxy/"))

(defun goto-zg()
  "goto zhanguo's home dir"
  (interactive)
  (find-file "f:/zg/logic/script/"))

(defun goto-sg()
  "goto sg's home dir"
  (interactive)
  (find-file "E:/game/logic/test/dyj"))

;; ≤Â»Î«©√˚
(defun insert-sign ()
  (interactive)
  (insert (format-time-string " %Y-%m-%d %H:%M:%S by BlueQ" (current-time))))

(defun lua-file-header()
  (interactive)
  (progn
   (insert "------------------------------------------------------\n")
   (insert "-- file  :\n")
   (insert "-- desc  :\n")
   (insert "-- author:") (insert-sign) (insert "\n")
   (insert "------------------------------------------------------\n")))

(defun python-file-header()
  (interactive)
  (progn
    (insert "# ----------------------------------------------------\n")
    (insert "# $URL:$\n")
    (insert "# $Desc:$\n")
    (insert "# $Author:$\n")
    (insert "# ----------------------------------------------------\n")))

(defun lua-function-header()
  (interactive)
  (progn
    (insert "-----------------------------------------------------\n")
    (insert "-- name   :\n")
    (insert "-- desc   :\n")
    (insert "-- args   :\n")
    (insert "-- returns:\n")
    (insert "--") (insert-sign) (insert "\n")
    (insert "-----------------------------------------------------\n")))

(define-key global-map (kbd "M-'") 'my-toggle-selective-display)
(defun my-toggle-selective-display()
  "set-selective-display to current column or toggle
selective-display --lgfang"
  (interactive)
  (let ((arg (progn (back-to-indentation) (current-column))))
    (set-selective-display (if (eq arg selective-display) nil arg))))
