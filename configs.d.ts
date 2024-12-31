/* It was an inappropriate definition of json, but better than json schema. */

export interface Root {
	servers: Server[];
}

export interface Server {
	server_names: string[];
	listen: string[];
	client_max_body_size: string;
	error_pages: ErrorPage[];
	locations: Location[];
}

export interface ErrorPage {
	code: number;
	path: string;
}

export interface Location {
	path: string;
	type: "default" | "upload" | "redirect";
	allow_methods?: string[];
}

export interface LocationDefault extends Location {
	type: "default";
	root: string;
	index: string[];
	autoindex: boolean;
	cgi?: { [ext: string]: Cgi };
}

export interface Cgi {
	bin: string;
	// Future authority, etc.
}

export interface LocationUpload extends Location {
	type: "upload";
	store: string;
	// Future authority, etc.
}

export interface LocationRedirect extends Location {
	type: "redirect";
	code: number;
	redirect: string;
}
