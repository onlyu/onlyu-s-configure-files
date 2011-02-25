(provide 'onlyu-fs)
(defvar fs-dir "F:/fs/trunk/")
(defvar fs-script-dir (concat fs-dir "script") )
(defun fs-goto-script()
  ""
  (interactive)
  ;(setq tags-file-name "j:/xy2/script/TAGS")
  (find-file fs-script-dir))

(defun fs-run(file)
  ""
  (interactive)
  (progn (setq old-default-directory default-directory) ; 
	 (setq default-directory fs-dir)		; 
	 (start-file-process "fsgame-client" "fsgame-client" (concat fs-dir file))
	 (setq default-directory old-default-directory))) ; 
;; (defun fs-run-bat(file)
;;   ""
;;   (interactive)
;;   (progn (setq old-default-directory default-directory)
;; 	 (setq default-directory )
;; 	 (start-file-process "fs-start", "fs-start" (concat current-directory file))
;; 	 (setq default-directory old-default-directory)))

(defun fs-start()
  ""
  (interactive)
  (fs-run "start.bat"))

(defun fs-lua-editor()
  ""
  (interactive)
  (fs-run "luaeditor.bat"))

(defun fs-python-editor()
  ""
  (interactive)
  (fs-run "pyeditor.bat"))

(global-set-key [f9] 'fs-start)
(global-set-key [f10] 'fs-lua-editor)
(global-set-key [f11] 'fs-python-editor)