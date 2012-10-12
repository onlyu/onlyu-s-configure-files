(add-to-list 'load-path
	     "~/emacs/vendor/yasnippet")

(require 'yasnippet) ;; not yasnippet-bundle
(yas/initialize)
(yas/load-directory "~/emacs/vendor/yasnippet/snippets")
;; (setq yas/prompt-functions '(yas/dropdown-prompt))
(setq yas/prompt-functions '(yas/ido-prompt))
(provide 'onlyu-yasnippet)
