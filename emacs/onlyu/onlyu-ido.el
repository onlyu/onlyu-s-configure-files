(require 'ido)
(ido-mode)

(setq ido-ignore-files
      (append '("\\`.*\\.d" "\\`.*\\.d\\..*" "\\`.*flymake.*")
	    ido-ignore-files))

(provide 'onlyu-ido)
