export interface Root {
	servers: Server[]
}

export interface Server {
	server_names: string[]
	listen: string[]
	client_max_body_size: string
	error_page: ErrorPage[]
	locations: Location[]
}

export interface ErrorPage {
	code: number
	path: string
}

export interface Location {
	path: string
	type: "default" | "upload" | "redirect"
	allow_methods?: string[]
}

export interface LocationDefault extends Location {
	root: string
	index: string[]
	autoindex: boolean
	cgi?: Cgi[]
}

export interface Cgi {
	ext: string
	bin: string
	args?: string[]
}

export interface LocationUpload extends Location {
	store: string
}

export interface LocationRedirect extends Location {
	code: number
	redirect: string
}
