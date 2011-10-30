(require 'flymake)

(setq flymake-allowed-file-name-masks
      (cons '(".+\\.c$"
	      flymake-simple-make-init
	      flymake-simple-cleanup
	      flymake-get-real-file-name)
	    flymake-allowed-file-name-masks))

(defun of-flymake-error ()
  (interactive)
  (let* ((line-no             (flymake-current-line-no))
	 (line-err-info-list  (nth 0 (flymake-find-err-info flymake-err-info line-no)))
	 (menu-data           (flymake-make-err-menu-data line-no line-err-info-list))
	 (choice              nil))
    (if menu-data
	(progn
	  (setq choice (flymake-popup-menu menu-data))
	  (flymake-log 3 "choice=%s" choice)
	  (when choice
	    (eval choice)))
      (flymake-goto-next-error))))

;; use \C-ce for flymake
(define-key c-mode-map "\C-ce" 'of-flymake-error)
(add-hook 'find-file-hooks 'flymake-find-file-hook)

(provide 'onlyu-flymake)