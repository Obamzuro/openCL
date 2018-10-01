/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorAdd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 17:33:48 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/01 14:09:21 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

__kernel void SAXPY(__global float *x, __global float *y,
		float a)
{
	int		iGid;

	iGid = get_global_id(0);
//	if (iGid >= iNumElements)
//		return ;
	y[iGid] += a * x[iGid];
}
