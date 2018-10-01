/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 17:32:28 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/01 18:41:45 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenCL/opencl.h>
#include <stdio.h>
#include "libft.h"
#include "ft_printf.h"

char	*LoadKernel(const char *name)
{
	int		fd;
	char	*line;
	char	*temp;
	char	*ret;
	int		i;

	fd = open(name, O_RDONLY);
	line = NULL;
	ret = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		temp = ret;
		ret = ft_strjoin(ret, line);
		free(temp);
		free(line);
	}
	close(fd);
	return (ret);
}

cl_program	CreateProgram(char *source, cl_context context)
{
	size_t	lengths[1] = { ft_strlen(source) };
	const char	*sources[1] = { source };
	cl_int	error = 0;

	cl_program program = clCreateProgramWithSource(context, 1, sources, NULL, &error);
	if (error != CL_SUCCESS)
		printf("Error: CreateProgramWS %d\n", error);
	return (program);
}

void	getPlatformInfo(cl_platform_id platform, cl_platform_info param)
{
	char	*buffer;
	size_t	sizeneeded;
	cl_int	status;

	status = clGetPlatformInfo(platform, param, 0, NULL, &sizeneeded);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: getPlatformInfo\n", 23);
		return ;
	}
	buffer = (char *)malloc(sizeneeded);
	status = clGetPlatformInfo(platform, param, sizeneeded, buffer, NULL);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: getPlatformInfo\n", 23);
		return ;
	}
	write(1, buffer, sizeneeded);
	write(1, "\n", 1);
	free(buffer);
}

void	getDeviceInfo(cl_device_id device, cl_platform_info param)
{
	char			*buffer;
	size_t			sizeneeded;
	cl_int			status;
	cl_device_type	devtype;

	if (param == CL_DEVICE_TYPE)
	{
		status = clGetDeviceInfo(device, param, sizeof(cl_device_type),
				&devtype, NULL);
		if (status != CL_SUCCESS)
		{
			write(2, "Error: getDeviceInfo\n", 21);
			return ;
		}
		if (devtype == CL_DEVICE_TYPE_GPU)
			ft_printf("GPU\n");
		else if (devtype == CL_DEVICE_TYPE_CPU)
			ft_printf("CPU\n");
		else if (devtype == CL_DEVICE_TYPE_ACCELERATOR)
			ft_printf("ACCELERATOR\n");
		else
			ft_printf("UNDEFINED TYPE\n");
	}
	else
	{
		status = clGetDeviceInfo(device, param, 0, NULL, &sizeneeded);
		if (status != CL_SUCCESS)
		{
			write(2, "Error: getDeviceInfo\n", 21);
			return ;
		}
		buffer = (char *)malloc(sizeneeded);
		status = clGetDeviceInfo(device, param, sizeneeded, buffer, NULL);
		if (status != CL_SUCCESS)
		{
			write(2, "Error: getDeviceInfo\n", 21);
			return ;
		}
		write(1, buffer, sizeneeded);
		write(1, "\n", 1);
		free(buffer);
	}
}

int		main(void)
{
	cl_uint					numPlatforms;
	cl_int					status;
	cl_platform_id			*platforms;
	cl_platform_id			platform;

	cl_uint					numDevices;
	cl_device_id			*devices;
	cl_device_id			device;

	cl_context				context;
	cl_int					error = CL_SUCCESS;

	platform = NULL;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: Getting Platforms.\n", 26);
		return (-1);
	}
	if (numPlatforms > 0)
	{
		platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		if (status != CL_SUCCESS)
		{
			write(2, "Error: Getting Platform Ids.\n", 29);
			return (-1);
		}
		platform = platforms[0];
		int i = -1;
		while (++i < numPlatforms)
		{
			ft_printf("Platform #%d\n", i + 1);
			getPlatformInfo(platforms[i], CL_PLATFORM_PROFILE);
			getPlatformInfo(platforms[i], CL_PLATFORM_VERSION);
			getPlatformInfo(platforms[i], CL_PLATFORM_NAME);
			getPlatformInfo(platforms[i], CL_PLATFORM_VENDOR);
			getPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS);
			ft_printf("\n");
		}
		free(platforms);
	}

	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: Getting devices.\n", 26);
		return (-1);
	}
	if (numDevices > 0)
	{
		devices = (cl_device_id *)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
		if (status != CL_SUCCESS)
		{
			write(2, "Error: Getting Device Ids.\n", 29);
			return (-1);
		}
		device = devices[0];
		int i = -1;
		while (++i < numDevices)
		{
			ft_printf("Device #%d\n", i + 1);
			getDeviceInfo(devices[i], CL_DEVICE_TYPE);
			getDeviceInfo(devices[i], CL_DEVICE_NAME);
			getDeviceInfo(devices[i], CL_DEVICE_VENDOR);
			getDeviceInfo(devices[i], CL_DEVICE_VERSION);
			getDeviceInfo(devices[i], CL_DEVICE_PROFILE);
			ft_printf("\n");
		}
//		free(devices);
	}

//	const cl_context_properties contextProperties[] =
//	{CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0, 0};
	context = clCreateContext(NULL,
			numDevices, devices, 0, 0, &error);
	if (error != CL_SUCCESS)
	{
		write(2, "Error: Create context.\n", 23);
		return (-1);
	}
	write(1, "Context created!\n", 17);

	const int testDataSize = 1 << 10;
	float	a[testDataSize];
	float	b[testDataSize];
	int i = -1;
	while (++i < testDataSize)
	{
		a[i] = (float)(23 ^ i);
		b[i] = (float)(42 ^ i);
	}
	cl_mem aBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY |
			CL_MEM_COPY_HOST_PTR, sizeof(float) * testDataSize,
			a, NULL);
	cl_mem bBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY |
			CL_MEM_COPY_HOST_PTR, sizeof(float) * testDataSize,
			b, NULL);

	cl_program program = CreateProgram(LoadKernel("source/saxpy.cl"), context);
	status = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: Build program.\n", 22);
		return (-1);
	}

	cl_kernel kernel = clCreateKernel(program, "SAXPY", &error);
	if (error != CL_SUCCESS)
	{
		write(2, "Error: create Kernel.\n", 22);
		return (-1);
	}

	cl_command_queue queue = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS)
	{
		write(2, "Error: create Qeueue.\n", 22);
		return (-1);
	}

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &aBuffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &bBuffer);
	static const float two = 2.0f;
	clSetKernelArg(kernel, 2, sizeof(float), &two);

	const size_t globalWorkSize[] = {testDataSize, 0, 0};
	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: Enqueue range.\n", 22);
		return (-1);
	}

	status = clEnqueueReadBuffer(queue, bBuffer, CL_TRUE, 0, sizeof(float) * testDataSize, b, 0, NULL, NULL);
	if (status != CL_SUCCESS)
	{
		write(2, "Error: Enqueue read.\n", 21);
		return (-1);
	}

	clReleaseCommandQueue(queue);

	clReleaseMemObject(bBuffer);
	clReleaseMemObject(aBuffer);

	clReleaseKernel(kernel);
	clReleaseProgram(program);

	clReleaseContext(context);
}
