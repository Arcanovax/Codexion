/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthetcha <mthetcha@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:02:48 by mthetcha          #+#    #+#             */
/*   Updated: 2026/03/16 11:06:35 by mthetcha         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int take_dongles(t_coder *coder)
{
	coder->left->is_used = 1;
	print_msg("has taken a dongle", coder);
	coder->right->is_used = 1;
	print_msg("has taken a dongle", coder);
	return (1);
}
int leave_dongles(t_coder *coder)
{

	coder->left->is_used = 0;
	coder->left->last_use = get_time(coder->all);
	coder->right->is_used = 0;
	coder->right->last_use = get_time(coder->all);

	return (1);
}
