; custom settings
(add-to-list 'load-path "~/emacs/onlyu/")
(add-to-list 'load-path "~/emacs/vendor/")


;; (semantic-mode 1)
(require 'lpc-mode)

(defcustom onlyu-need-erlang nil
  "")

(require 'onlyu-custom)
(require 'fs-server)
(require 'onlyu-key)
(require 'onlyu-alias)
(require 'onlyu-linum)
(require 'onlyu-fs)
(require 'onlyu-ido)
(require 'onlyu-egg)
(require 'onlyu-psvn)
(require 'onlyu-xcscope)
(require 'onlyu-dict)
(require 'onlyu-auto-complete)
(require 'onlyu-yasnippet)

(require 'onlyu-lua-mode)
(require 'onlyu-cc-mode)
(require 'onlyu-hs)

(require 'onlyu-dired-mode)
(require 'onlyu-flymake)

(require 'onlyu-find-func)
(require 'onlyu-util)
(require 'onlyu-locale-settings)
(require 'onlyu-iedit)
(require 'onlyu-clang)

(if onlyu-need-erlang
    (require 'onlyu-erlang))

;; (catch (require 'elixir-mode))

;; (semantic-mode 1)
;; init
(provide 'init)
