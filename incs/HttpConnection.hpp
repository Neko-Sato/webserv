/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:50:36 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 08:57:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/*
        httpコネクションはステートマシンだ
        何かを受信するたびにコールバックが呼び出される
        うまいこといけば次のステートに進む
*/

class HttpConnection {
private:
  enum State {
    READY,
    RECVE_REQUEST,
    RECVE_HEADER,
    RECVE_BODY,
    SEND_RESPONSE,
    FINISHED
  };

  State _state;

public:
  HttpConnection();
  ~HttpConnection();
};
