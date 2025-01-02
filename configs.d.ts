/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configs.d.ts                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:18:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 02:18:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* It was an inappropriate definition of json, but better than json schema. */

export interface Root {
	servers?: Server[];
}

export interface Server {
	server_names?: string[];
	listen?: Address[];
	client_max_body_size?: string;
	error_pages?: ErrorPage[];
	locations?: Location[];
}

export interface Address {
	host?: string;
	port: number;
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
	index?: string[];
	autoindex?: boolean;
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
	code?: number;
	redirect: string;
}
