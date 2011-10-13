;; get-url-content.el - Helper function to get URL content

;; Copyright (C) 2002 Sami Salkosuo
;; Author: Sami Salkosuo 
;; Version: 0.1 Sat Mar 16 12:16:01 2002

;; This file is not part of GNU Emacs.

;; GNU Emacs is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;; Commentary:
;;
;; Helper function to get URL content
;;
;; Installation
;;
;; Add get-url-content.el to your load path and add
;; (load "get-url-content") in .emacs file.
;;
;; Usage
;;
;; In any program where some content is get from URL.
;; For example, to get HTTP1.1 specification
;; ...
;; (setq http-spec (get-url-content "www.ietf.org/rfc/rfc2616.txt"))
;;
;; If you are behind firewalll, HTTP proxy and port must be specified
;; (setq http-spec (get-url-content "www.ietf.org/rfc/rfc2616.txt" proxy.host 8080))
;;

(defun get-url-content (url &optional proxy-host proxy-port)
  "Functions that gets content from specified URL. Returns content of
the URL
as a string. Supports only HTTP protocol.
Arguments:
url: get content from this URL. MUST be in format
'some.host[:port]/somefile?some=parameter'
proxy-host and proxy-port: HTTP proxy, host as string and port as integer"
  (let(
       (host)
       (port 80)
       (file)
       (buffer)
       (tcp-connection)
       (request)
       (tmp)
       (tmp2)
       (content)
       )
    (if proxy-host
	(progn
	    (setq host proxy-host)
	      (setq port proxy-port)
	        (setq file (concat "http://" url))
		  )
      (progn
	(setq tmp (string-match ":" url))
	(if (not tmp)
	        (setq tmp2 (string-match "/" url))
	    (setq tmp2 (string-match "/" url tmp))
	      )

	(if (not tmp)
	        (progn
		        (setq port 80)
			      (if (not tmp2)
				    (progn
				          (setq host url)
					      (setq file "/")
					          )
				(progn
				    (setq host (substring url 0 tmp2))
				      (setq file (substring url tmp2))
				        )
				)
			            )
	    (progn
	          (setq host (substring url 0 tmp))
		      (if (not tmp2)
			  (setq port (substring url (1+ tmp)))
			      (setq port (substring url (1+ tmp) tmp2))
			            )
		          )
	      )
	)
      )

    (setq buffer (get-buffer-create "*Get URL Content*"))
    (set-buffer buffer)
    (erase-buffer)
    (goto-char 0)

    (setq tcp-connection
	    (open-network-stream
	        "GET process-name"
		   buffer
		      host
		         port
			    ))

    (set-marker (process-mark tcp-connection) (point-min))
    (set-process-sentinel tcp-connection 'get-url-content-sentinel)

    (setq request (concat "GET " file " HTTP/1.0\n\n"))
    (process-send-string tcp-connection request)
    (get-url-content-parse tcp-connection)
    (delete-process tcp-connection)

    (setq content (buffer-string))
    (kill-buffer buffer)
    content
    )
  )

(defun get-url-content-sentinel (process string)
  "Process the results from the efine network connection.
process - The process object that is being notified.
string - The string that describes the notification."
  )

(defun get-url-content-parse (process)
  ""
  (let (
	(buffer)
	(header-end)
	(msg)
	(i)
	)
    (setq i 0)
    (while (eq (process-status process) 'open)
      (sit-for 0 200)
      )
    )
  )
(provide 'get-url-content)